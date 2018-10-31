import java.util.ArrayList;
import java.util.List;

public class InnerNode<T> extends Node<T> {
	private List<Node<T>> children;
	private InnerNode<T> parent;

	public InnerNode() {
		super();
		this.children = new ArrayList<Node<T>>();
		this.parent = null;
	}

	public Node<T> getChild(int index) {
		return this.children.get(index);
	}

	public void addChild(Node<T> node) {
		this.children.add(node);
	}
	
	public boolean hasParent() {
		return this.parent != null;
	}

	/**
	 * @return the children
	 */
	public List<Node<T>> getChildren() {
		return children;
	}

	/**
	 * @return the parent
	 */
	public InnerNode<T> getParent() {
		return parent;
	}

	/**
	 * @param children the children to set
	 */
	public void setChildren(List<Node<T>> children) {
		this.children = children;
	}

	/**
	 * @param parent the parent to set
	 */
	public void setParent(InnerNode<T> parent) {
		this.parent = parent;
	}

}