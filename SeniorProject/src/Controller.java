import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class Controller implements ControlInterface, ActionListener{
	//We implement keylistner becuase the key listner is going to be the main driver of the controller

	private View view;
	private Model model;
	private CarController carControl;
	
	
	//Strings for the action commands 
	//Each string represents a different button being clicked
	public final String DONUTBUTTON = "DONUT";
	public final String LOCBUTTON = "LOC";
	public final String MANBUTTON = "MAN";
	public final String AUTOBUTTON = "ATM";
	public final String RETBUTTON = "RET";
	
	//booleans to indicate wheter a button is curently being pressed
	private boolean upPressed;
	private boolean downPressed;
	private boolean leftPressed;
	private boolean rightPressed;
	private boolean escapePressed;
	
	
	public Controller(){
		view = new View(this);
		model = new Model();
		carControl =  new CarController(model);
		upPressed = false;
		downPressed = false;
		leftPressed = false;
		rightPressed = false;
		escapePressed =  false;

	}
	
	//this method should be called to kick off the whole process
	//Should include any set up or configuration code 
	//I.E tell the view the first thing to draw or set up the sockets or intialize any data or files in the model
	public void start(){
		
	}
	
	//Methods to handel the different key presses
	public void upPressed(){
		if(!upPressed){
			carControl.keyPressed(Keys.UPARROW);
			upPressed = true;
		}
	}
	
	public void upReleased(){
		upPressed = false;
		carControl.keyReleased(Keys.UPARROW);
	}
	
	public void downPressed(){
		if(!downPressed){
			carControl.keyPressed(Keys.DOWNARROW);
			downPressed = true;
		}
	}
	
	public void downReleased(){
		downPressed = false;
		carControl.keyReleased(Keys.DOWNARROW);
	}
	
	public void leftPressed(){
		if(!leftPressed){
			carControl.keyPressed(Keys.LEFTARROW);
			leftPressed = true;
		}
	}
	
	public void leftReleased(){
		leftPressed = false;
		carControl.keyReleased(Keys.LEFTARROW);
	}
	
	
	public void rightPressed(){
		if(!rightPressed){
			carControl.keyPressed(Keys.RIGHTARROW);
			rightPressed = true;
		}
	}
	
	public void rightReleased(){
		rightPressed = false;
		carControl.keyReleased(Keys.RIGHTARROW);

	}
	
	public void escapePressed(){
		if(!escapePressed){
			carControl.keyPressed(Keys.ESCAPE);
			escapePressed = true;
		}
	}
	
	public void escapeReleased(){
		escapePressed = false;
	}

	
	

	//this method handles buttons being pressed on the screen
	public void actionPerformed(ActionEvent arg0) {
		if(arg0.getActionCommand().equals(DONUTBUTTON)){
			carControl.doDonuts();
		}
		else if(arg0.getActionCommand().equals(LOCBUTTON)){
			carControl.location();
		}
		else if(arg0.getActionCommand().equals(MANBUTTON)){
			carControl.setManual();
			view.dispManualMode(); //update the view to indicate in manual mode
		}
		else if(arg0.getActionCommand().equals(AUTOBUTTON)){
			carControl.setAutomatic();
			view.dispAutoMode(); //update the view to indicate in automatic mode
		}
		else if(arg0.getActionCommand().equals(RETBUTTON)){
			carControl.returnBack();
		}
		else{
			//do nothing
		}
		
		
	}


}
