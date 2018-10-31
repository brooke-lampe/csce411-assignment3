
public class LeafNode<T> extends Node<T> {

	public LeafNode() {
		super();
	}

	LeafNode<T> addData(T key) {
		if (this.size() == BPlusTree.FAN_OUT) {
			LeafNode<T> newLeaf = new LeafNode<T>();
			newLeaf.addKey(key);
			this.setNext(newLeaf);
			newLeaf.setPrevious(this);
			return newLeaf;
		}
		this.addKey(key);
		return null;
	}

	public int countKeys(T key) {
		LeafNode<T> current = this;
		int count = 0;
		boolean found = false;
		int i = 0;
		while (true) {
			for (T nodeKey : current.getKeys()) {
				if (BPlusTree.compareUserLocation((String) nodeKey, (String) key) == 0) {
					found = true;
					count++;
				} else if (found) {
					found = false;
					break;
				}
			}
			if (!found && i > 1) {
				break;
			}
			current = (LeafNode<T>) current.getNext();
			i++;
		}
		return count;
	}
}
