import java.io.BufferedReader;
import java.io.FileReader;

public class JSONSeparator{

	private static void PrintContaining(String fileName, String source){
		try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
    		String line;
    		String jsonSource = "\"Source\":\"" + source + "\"";
    		while ((line = br.readLine()) != null) {
				if(line.contains(jsonSource)){
					System.out.println(line);
				}
    		}
		}
		catch(Exception e){
			e.printStackTrace(System.out);
		}
	}

	public static void main(String [] args){
		if(args.length < 2 || args.length > 2){
			System.out.println("");
			System.out.println("Welcome to JSON Separator!");
			System.out.println("Created by Jacob Chesley for the UC Astro Cats Data Extraction");
			System.out.println("");
			System.out.println("Usage:");
			System.out.println("java JSONSeparator fileName contains");
		}
		else{
			PrintContaining(args[0], args[1]);
		}
	}
}