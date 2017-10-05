
import javax.swing.JFrame;

public class MyFrame extends JFrame{
	//my frame extends JFrame so that we can use it's methods, and have our own unique JFrame
	//Mainly to give it our custom key listener
	JFrame frame;
	
	public MyFrame(Controller c){
		//can call all three of these methods without issue becase we extend JFrame
		addKeyListener(c); //this tells us that the key events will be picked up in the INputKeyListener class
		setFocusable(true);
		setFocusTraversalKeysEnabled(false);
	
	}
	



}
