import java.util.ArrayList;
import java.util.List;

abstract class Node<T> {
	private List<T> keys;
	private Node<T> next;
	private Node<T> previous; // For debugging

	public Node() {
		this.keys = new ArrayList<T>();
		this.next = null;
	}

	public int size() {
		return this.keys.size();
	}

	public void addKey(T key) {
		this.keys.add(key);
	}

	public T getKey(int index) {
		return this.keys.get(index);
	}

	public void setKeys(List<T> keys) {
		this.keys = keys;
	}

	public List<T> getKeys() {
		return this.keys;
	}

	/**
	 * @return the next
	 */
	public Node<T> getNext() {
		return next;
	}

	public void setNext(Node<T> next) {
		this.next = next;
	}

	/**
	 * @return the previous
	 */
	public Node<T> getPrevious() {
		return previous;
	}

	/**
	 * @param previous
	 *            the previous to set
	 */
	public void setPrevious(Node<T> previous) {
		this.previous = previous;
	}

}
