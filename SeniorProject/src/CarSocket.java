import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class CarSocket {
	private static final String ADDRS = "192.168.88.88";
	private static final int PORT = 333; //Need to get the port were using 
	private Socket carSock;
	private BufferedReader in;
	private PrintWriter out;
	
	//Pre:Need to connect to the pasive socket on the car
	//Post: The passive socket has been connected to 
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
	
	//Pre:Need to write a string s to the socket
	//Post: S has been written to the socket
	public void write(String s){
		out.println(s);
	}
	
	//Pre: Need to read from the socket
	//Post: Returns the the string read from the socket
	public String read(){
		String result = "";
		try {
			result  = in.readLine();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return result;
	}
	
	//Pre: Must close the socket connedtion
	//Post: The socket connection has been clsosed 
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
