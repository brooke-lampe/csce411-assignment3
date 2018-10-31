import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

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

	public Set<Integer> getUsers(T key) {
		LeafNode<T> current = this;
		Set<Integer> users = new HashSet<Integer>();
		while (current != null) {
			for (T nodeKey : current.getKeys()) {
				String fileName = (String) nodeKey;
				try {
					BufferedReader reader = new BufferedReader(new FileReader(fileName));
					String[] lineArray = reader.readLine().split("\t");
					reader.close();
					if (lineArray.length >= 3) {
						String[] locationArray = lineArray[2].split(",");
						if (locationArray.length >= 2) {
							if (((String) key).compareTo(locationArray[1]) == 0) {
								users.add(Integer.parseInt(lineArray[0]));
							}
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
			}
			current = (LeafNode<T>) current.getNext();
			if (Compare.userByLocation((String) key, (String) current.getKey(0)) < 0)
				break;
		}
		return users;
	}

	public Map<Integer, Integer> countMessages() {
		LeafNode<T> current = this;
		Map<Integer, Integer> users = new HashMap<Integer, Integer>();
		while (current != null) {
			for (T nodeKey : current.getKeys()) {
				String fileName = (String) nodeKey;
				try {
					BufferedReader reader = new BufferedReader(new FileReader(fileName));
					String line = reader.readLine();
					while (line != null) {
						String[] lineArray = line.split("\t");
						String[] time = lineArray[1].split(" ")[1].split(":");
						if (Integer.parseInt(time[0]) == 8
								|| (Integer.parseInt(time[0]) == 9 && Integer.parseInt(time[1]) == 0)) {
							int userId = Integer.parseInt(lineArray[0]);
							if (users.containsKey(userId)) {
								users.put(userId, users.get(userId) + 1);
							} else {
								users.put(userId, 1);
							}
						}
						line = reader.readLine();
					}
					reader.close();
				} catch (FileNotFoundException e) {
					System.err.printf("Could not read file %s", fileName);
					System.exit(0);
				} catch (IOException e) {
					System.err.printf("Could not read lines of %s", fileName);
					System.exit(0);
				}
			}
			current = (LeafNode<T>) current.getNext();
			if (Compare.messageByTime((String) current.getKey(0)) < 0)
				break;
		}
		return users;
	}
}
