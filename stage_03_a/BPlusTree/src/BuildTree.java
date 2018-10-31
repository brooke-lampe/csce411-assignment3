import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class BuildTree {

	public static void main(String[] args) {
		String[] usersFileNames = new String[2000];
		String[] messagesFileNames = new String[2000];
		for (int i = 0; i < usersFileNames.length; i++) {
			usersFileNames[i] = String.format("data/users/user_%06d.dat", i);
			messagesFileNames[i] = String.format("data/messages/message_%06d.dat", i);
		}
		long start = System.currentTimeMillis();
		BPlusTree<String> usersTree = buildTree(usersFileNames);
		BPlusTree<String> messagesTree = buildTree(messagesFileNames);
		long elapsedTime = System.currentTimeMillis() - start;
		System.out.printf("Two B Plus Trees are created: one for users and one for messages\n");
		System.out.print("Processing Time " + elapsedTime / 1000F + "\n\n");
		// ---------------------------------------------------------------------------------------------

		// ----- Step B ----
		start = System.currentTimeMillis();
		LeafNode<String> resultNode = usersTree.searchUserByLocation("Nebraska");
		Set<Integer> users = resultNode.getUsers("Nebraska");
		elapsedTime = System.currentTimeMillis() - start;
		System.out.printf("The number of users who are from Nebraska is %d\n", users.size());
		System.out.print("Processing Time " + elapsedTime / 1000F + "\n\n");

		start = System.currentTimeMillis();
		resultNode = messagesTree.searchMessageByTime();
		Map<Integer, Integer> userMessagesMap = resultNode.countMessages();
		elapsedTime = System.currentTimeMillis() - start;
		System.out.printf("The number of users who sent messages between 8am and 9am is %d\n", userMessagesMap.size());
		System.out.print("Processing Time " + elapsedTime / 1000F + "\n\n");

	}

	private static BPlusTree<String> buildTree(String[] keys) {
		List<LeafNode<String>> leafNodes = new ArrayList<LeafNode<String>>();
		LeafNode<String> currentLeafNode = new LeafNode<String>();
		currentLeafNode.addData(keys[0]);
		leafNodes.add(currentLeafNode);
		InnerNode<String> rightMostInnerNode = new InnerNode<String>();
		rightMostInnerNode.addChild(currentLeafNode);

		BPlusTree<String> tree = new BPlusTree<String>(rightMostInnerNode);
		for (int i = 1; i < keys.length; i++) {
			String key = keys[i];
			LeafNode<String> newLeaf = (LeafNode<String>) currentLeafNode.addData(key);
			if (newLeaf != null) {
				leafNodes.add(newLeaf);
				rightMostInnerNode = tree.addData(rightMostInnerNode, newLeaf.getKey(0), newLeaf);
				currentLeafNode = newLeaf;
			}
		}
		return tree;
	}

}
