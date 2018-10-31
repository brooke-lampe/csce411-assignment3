import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class BPlusTree<T> {
	protected static final int FAN_OUT = 200;
	private InnerNode<T> root;

	public BPlusTree(InnerNode<T> root) {
		this.root = root;
	}

	InnerNode<T> addData(InnerNode<T> node, T data, Node<T> child) {
		node.addKey(data);
		node.addChild(child);
		if (node.size() > BPlusTree.FAN_OUT) {
			return this.splitNode(node);
		}
		return node;

	}

	public LeafNode<T> search(T key) {
		return this.search(key, this.root);
	}

	private LeafNode<T> search(T key, Node<T> node) {
		if (node instanceof LeafNode) {
			return (LeafNode<T>) node;
		}
		int i = 0;
		while (i != node.size()) {
			if (compareUserLocation((String) node.getKey(i), (String) key) <= 0) {
				break;
			}
			i++;
		}
		return this.search(key, ((InnerNode<T>) node).getChild(i));
	}

	public static int compareUserLocation(String fileName, String key) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(fileName));
			String[] location = reader.readLine().split("\t");
			reader.close();
			if (location.length >= 3) {
				String[] locationArray = location[2].split(",");
				if (locationArray.length >= 2) {
					return key.compareTo(locationArray[1]);
				}
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

	/**
	 * @param node
	 *            the inner node to be split
	 * @return the right most inner node
	 */
	private InnerNode<T> splitNode(InnerNode<T> node) {
		int middle = node.size() / 2;
		T splitData = node.getKey(middle);

		// Extracting the first half of keys children lists
		List<T> newKeysList = new ArrayList<T>();
		List<Node<T>> newChildren = new ArrayList<Node<T>>();
		for (int i = 0; i < middle; i++) {
			newKeysList.add(node.getKey(i));
			newChildren.add(node.getChild(i));
		}
		newChildren.add(node.getChild(middle));

		// creating a new inner node to the right of the given node and filling it with
		// the second half of the keys and children
		InnerNode<T> siblingNode = new InnerNode<T>();
		for (int i = middle + 1; i < node.size(); i++) {
			this.addData(siblingNode, node.getKey(i), node.getChild(i));
		}
		siblingNode.addChild(node.getChild(node.getChildren().size() - 1));

		// update the given node's keys and children lists
		node.setKeys(newKeysList);
		node.setChildren(newChildren);
		node.setNext(siblingNode);
		siblingNode.setPrevious(node);

		// Insert the split data into the parent node if it exists
		if (node.hasParent()) {
			siblingNode.setParent(this.addData(node.getParent(), splitData, siblingNode));
		} else { // All nodes are full, so create a new root with the split value
			InnerNode<T> newRoot = new InnerNode<T>();
			this.addData(newRoot, splitData, node);
			newRoot.addChild(siblingNode);
			node.setParent(newRoot);
			siblingNode.setParent(newRoot);
			this.root = newRoot;
		}
		return siblingNode;
	}

	public void print() {
		Node<T> currentNode = this.root;
		while (true) {
			for (int i = 0; i < currentNode.size(); i++) {
				T key = currentNode.getKey(i);
				if (i == 0) {
					System.out.print("[");
				}
				System.out.print(key);
				if (i != currentNode.size() - 1) {
					System.out.print(", ");
				} else {
					System.out.print("]");
				}
			}
			System.out.print("\t\t");
			if (currentNode.getNext() != null) {
				currentNode = currentNode.getNext();
			} else if (!(currentNode instanceof LeafNode)) {
				while (currentNode.getPrevious() != null) {
					currentNode = currentNode.getPrevious();
				}
				currentNode = ((InnerNode<T>) currentNode).getChild(0);
				System.out.print("\n");
			} else {
				break;
			}
		}

	}
}
