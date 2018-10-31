import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class BuildTree {

	public static void main(String[] args) {
		String[] usersFileNames = new String[2000];
		String[] messagesFileNames = new String[2000];
		for (int i = 0; i < usersFileNames.length; i++) {
			usersFileNames[i] = String.format("data/users/user_%06d.dat", i);
			messagesFileNames[i] = String.format("data/messages/message_%06d.dat", i);
		}

		BPlusTree<String> usersTree = buildTree(usersFileNames);
		BPlusTree<String> messagesTree = buildTree(messagesFileNames);

		LeafNode<String> resultNode = usersTree.search("Nebraska");
		int count = resultNode.countKeys("Nebraska");
		System.out.printf("Nebraska %d", count);

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
