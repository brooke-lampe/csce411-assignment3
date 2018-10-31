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
	}

	private static int compareUserLocation(String fileName, String key) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(fileName));
			String[] location = reader.readLine().split("\t");
			reader.close();
			if (location.length >= 3) {
				String state = location[2].split(",")[1];
				return key.compareTo(state);
			}
			reader.close();
		} catch (FileNotFoundException e) {
			System.err.printf("Could not read file %s", fileName);
			System.exit(0);
		} catch (IOException e) {
			System.err.printf("Could not read lines of %s", fileName);
			System.exit(0);
		}
		return 1;
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
