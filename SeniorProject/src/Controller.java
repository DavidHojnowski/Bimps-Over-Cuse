import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class Controller implements KeyListener{
	//We implement keylistner becuase the key listner is going to be the main driver of the controller

	View view;
	Model model;
	CarController carControl;
	
	//The key codes for up, down, left, and right
	private static final int UP=38;
	private static final int DOWN =40;
	private static final int LEFT = 37;
	private static final int RIGHT = 39;
	
	//booleans to indicate wheter a button is curently being pressed
	private boolean upPressed;
	private boolean downPressed;
	private boolean leftPressed;
	private boolean rightPressed;
	
	
	public Controller(){
		view = new View(this);
		model = new Model();
		carControl =  new CarController(model);
		upPressed = false;
		downPressed = false;
		leftPressed = false;
		rightPressed = false;

	}
	
	//this method should be called to kick off the whole process
	//Should include any set up or configuration code 
	//I.E tell the view the first thing to draw or set up the sockets or intialize any data or files in the model
	public void start(){
		
	}
	
	@Override
	public void keyPressed(KeyEvent arg0) {
		
		handleKeyPress(arg0.getKeyCode());
	
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		handleKeyRelease(arg0.getKeyCode());
	}

	//Have to include this method here but probably will never use it 
	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
		//System.out.println("Key Typed");
		
	}
	
	//this method then handles what to do once a key has been pressed
	private void handleKeyPress(int keyCode){
		switch(keyCode){
			case UP:
				if(!upPressed){
					carControl.keyPressed(Keys.UPARROW);
				}
				upPressed = true;
				break;
			case DOWN:
				if(!downPressed){
					carControl.keyPressed(Keys.DOWNARROW);
				}
				downPressed = true;
				break;
				
			case LEFT:
				if(!leftPressed){
					carControl.keyPressed(Keys.LEFTARROW);
				}
				leftPressed = true;
				break;
				
			case RIGHT:
				if(!rightPressed){
					carControl.keyPressed(Keys.RIGHTARROW);
				}
				rightPressed = true;
				break;
			default:
				//do something key pressed that we don't care about
				break;
				
		}
		
	}
	
	//this method handles what to do once a key has been relaseed
	private void handleKeyRelease(int keyCode){
		switch(keyCode){
			case UP:
				upPressed = false;
				carControl.keyReleased(Keys.UPARROW);
				break;
			case DOWN:
				downPressed = false;
				carControl.keyReleased(Keys.DOWNARROW);
				break;
			case LEFT:
				leftPressed = false;
				carControl.keyReleased(Keys.LEFTARROW);
				break;
			case RIGHT:
				rightPressed = false;
				carControl.keyReleased(Keys.RIGHTARROW);
				break;
			default:
				//may need to do something when keys we dont care about are relased
				break;
				
			
		}
		
	}
}
