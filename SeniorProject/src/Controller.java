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
	
	//Methods to handel the different key presses---------------------
	
	//Pre:The up arrow key has been pressed
	//Post: The up key press event has been handled
	public void upPressed(){
		if(!upPressed){
			carControl.keyPressed(Keys.UPARROW);
			upPressed = true;
		}
	}
	
	//Pre:The up key has been realsed
	//Post:The up key relase event has been handled
	public void upReleased(){
		upPressed = false;
		carControl.keyReleased(Keys.UPARROW);
	}
	
	//Pre: The down arrow has been pressed
	//Post: The down arrow key press has be handled
	public void downPressed(){
		if(!downPressed){
			carControl.keyPressed(Keys.DOWNARROW);
			downPressed = true;
		}
	}
	
	//Pre:The down key has been released
	//Post:The down key release event has been handled
	public void downReleased(){
		downPressed = false;
		carControl.keyReleased(Keys.DOWNARROW);
	}
	
	//Pre:The left key has been pressed
	//Post: The left key event has been handeled
	public void leftPressed(){
		if(!leftPressed){
			carControl.keyPressed(Keys.LEFTARROW);
			leftPressed = true;
		}
	}
	
	//Pre:The left key has been released
	//Post: The left key release event has been handled
	public void leftReleased(){
		leftPressed = false;
		carControl.keyReleased(Keys.LEFTARROW);
	}
	
	//Pre:The right key has been pressed
	//Post: The right key press event has been handled
	public void rightPressed(){
		if(!rightPressed){
			carControl.keyPressed(Keys.RIGHTARROW);
			rightPressed = true;
		}
	}
	
	//Pre: The right key has been relaeased
	//Post: The right key release event has been handled
	public void rightReleased(){
		rightPressed = false;
		carControl.keyReleased(Keys.RIGHTARROW);

	}
	
	//Pre:The escape key has been pressed
	//Post: The escape key press has beeen handled
	public void escapePressed(){
		if(!escapePressed){
			carControl.keyPressed(Keys.ESCAPE);
			escapePressed = true;
		}
	}
	
	//Pre: The escape key has been realesed
	//Post: The escape key release event has been handled
	public void escapeReleased(){
		escapePressed = false;
	}

	
	
	//Pre:A button has been pressed
	//Post: That button press has been handled 
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
