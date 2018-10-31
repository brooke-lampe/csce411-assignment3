
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
}
