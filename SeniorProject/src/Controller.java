import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class Controller implements KeyListener{
	//We implement keylistner becuase the key listner is going to be the main driver of the controller

	View view;
	Model model;
	
	public Controller(){
		view = new View(this);
		model = new Model();

	}
	
	//this method should be called to kick off the whole process
	//Should include any set up or configuration code 
	//I.E tell the view the first thing to draw or set up the sockets or intialize any data or files in the model
	public void start(){
		
	}
	
	@Override
	public void keyPressed(KeyEvent arg0) {
		// TODO Auto-generated method stub
		System.out.println("Key Pressed");
		//Now we actually have to do something with this 
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub
		System.out.println("Key Released");
	}

	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
		System.out.println("Key Typed");
		
	}
}
