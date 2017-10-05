import javax.swing.JFrame;

public class View {
	private MyFrame test; //this is gunna be the frame or the window that we draw in
	
	public View(Controller c){
		//This tells the frame the dimensions to close on exit and make it visible
		//the location of this code may change but for right now we will call it from here 
		MyFrame test = new MyFrame(c);
		test.setSize(600,600);
		test.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//test.pack();
		test.setVisible(true);
	
	}
	
}
