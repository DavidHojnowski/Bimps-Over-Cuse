import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class CarSocket {
	private static final String ADDRS = "192.168.88.88";
	private static final int PORT = 0; //Need to get the port were using 
	private Socket carSock;
	private BufferedReader in;
	private PrintWriter out;
	
	
	public void connect(){
		try {
			InetAddress  addrs = InetAddress.getByName(ADDRS);
			carSock = new Socket(addrs,PORT);
			
			in = new BufferedReader( new InputStreamReader(carSock.getInputStream()));
			
			out = new PrintWriter(carSock.getOutputStream(), true);
			
		} catch (UnknownHostException e) {
			e.printStackTrace();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void write(String s){
		out.println(s);
	}
	
	public String read(){
		String result = "";
		try {
			result  = in.readLine();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		finally{
			return result;
		}
	}
	
	
	public void closeConnection(){
		try {
			in.close();
			out.close();
			carSock.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	
	
	
	
}
