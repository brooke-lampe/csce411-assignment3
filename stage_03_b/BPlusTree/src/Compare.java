import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Compare {
	public static int userByLocation(String queryLocation, String fileName) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(fileName));
			String[] location = reader.readLine().split("\t");
			reader.close();
			if (location.length >= 3) {
				String[] locationArray = location[2].split(",");
				if (locationArray.length >= 2) {
					return queryLocation.compareTo(locationArray[1]);
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

	public static int messageByTime(String fileName) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(fileName));
			String line = reader.readLine();
			while (line != null) {
				String[] time = (line.split("\t")[1].split(" ")[1].split(":"));
				if (Integer.parseInt(time[0]) > 8
						&& !((Integer.parseInt(time[0]) == 9 && Integer.parseInt(time[1]) == 0))) {
					reader.close();
					return -1;
				} else if (Integer.parseInt(time[0]) == 8
						|| (Integer.parseInt(time[0]) == 9 && Integer.parseInt(time[1]) == 0)) {
					reader.close();
					return 0;
				}
				line = reader.readLine();
			}
			reader.close();
			return 1;
		} catch (FileNotFoundException e) {
			System.err.printf("Could not read file %s", fileName);
			System.exit(0);
		} catch (IOException e) {
			System.err.printf("Could not read lines of %s", fileName);
			System.exit(0);
		}
		return 1;
	}

}
