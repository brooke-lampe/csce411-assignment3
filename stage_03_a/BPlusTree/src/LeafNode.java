
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
		while (current != null) {
			for (T nodeKey : current.getKeys()) {
				if (BPlusTree.compareUserLocation((String) nodeKey, (String) key) == 0) {
					count++;
				}
			}
			current = (LeafNode<T>) current.getNext();
			if (BPlusTree.compareUserLocation((String) current.getKey(0), (String) key) < 0)
				break;
		}
		return count;
	}
}
