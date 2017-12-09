
import java.awt.event.ActionEvent;
import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.KeyStroke;

public class View implements ViewInterface{
	private JFrame test; //this is gunna be the frame or the window that we draw in
	//Pannel to add components to
	private JPanel myPanel;
	//Buttons
	private JButton donutButton;
	private JButton locationButton;
	private JButton manButton;
	private JButton autoButton;
	private JButton retButton;
	
	//String constants for the key bindings
	private final static String UPPRES = "up pressed";
	private final static String UPRELS = "up released";
	private final static String DOWNPRES = "down pressed";
	private final static String DOWNRELS ="down released";
	private final static String LEFTPRES = "left pressed";
	private final static String LEFTRELS ="left released";
	private final static String RIGHTPRES = "right pressed";
	private final static String RIGHTRELS ="right released";
	private final static String ESCPRES = "escape pressed";
	private final static String ESCRELS ="escape released";
	
	//controler to cal the control methods
	private Controller control;
	
	

	//Pre: Need to created an instance of the view
	//Post: An instance has been created with all the comoponets for that view
	public View(Controller c){
		control = c;
		//This tells the frame the dimensions to close on exit and make it visible
		//the location of this code may change but for right now we will call it from here 
		test = new JFrame();
		test.setSize(600,600);
		test.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//first add a panel to add buttons to
		myPanel = new JPanel();
		
		//-----------------------------------------------------
		//Key binding for arrow keys
		//UP ARROW
		//Call get input map to add  the keystroke to it
		//need when in foucesed window for the keybinding to work with the jPanel
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("UP"), UPPRES);
		//call the action map to tell it what to do with keystroke
		myPanel.getActionMap().put(UPPRES, upPressedAction);
		//release
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("released UP"), UPRELS);
		myPanel.getActionMap().put(UPRELS, upReleasedAction);
		
		//DOWN
		//PRESS
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("DOWN"), DOWNPRES);
		myPanel.getActionMap().put(DOWNPRES, downPressedAction);
		//RELEASE
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("released DOWN"), DOWNRELS);
		myPanel.getActionMap().put(DOWNRELS, downReleasedAction);
		
		//LEFT
		//PRESS
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("LEFT"), LEFTPRES);
		myPanel.getActionMap().put(LEFTPRES, leftPressedAction);
		//RELEASE
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("released LEFT"), LEFTRELS);
		myPanel.getActionMap().put(LEFTRELS, leftReleasedAction);
		
		
		//RIGHT
		//PRESS
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("RIGHT"), RIGHTPRES);
		myPanel.getActionMap().put(RIGHTPRES, rightPressedAction);
		//RELEASE
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("released RIGHT"), RIGHTRELS);
		myPanel.getActionMap().put(RIGHTRELS, rightReleasedAction);
		
		//ESCAPE
		//PRESS
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("ESCAPE"), ESCPRES);
		myPanel.getActionMap().put(ESCPRES, escapePressedAction);
		//RELEASE
		myPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("released ESCAPE"), ESCRELS);
		myPanel.getActionMap().put(ESCRELS, escapeReleasedAction);

	
		
		
		
		//---------------------------------------------------
		
		//DoDuntus button-----------------
	    donutButton = new JButton("Do Donuts");
		//add the action string so we know what button we press
		donutButton.setActionCommand(c.DONUTBUTTON);
		//add a listner to the button using the controller so we can detect in the controller
		donutButton.addActionListener(c);
		
		//GetLocation Button-----------------------
		locationButton = new JButton("Get Location");
		locationButton.setActionCommand(c.LOCBUTTON);
		locationButton.addActionListener(c);
		
		//Manual Button------------------------------
		manButton = new JButton("Set to Manual mode");
		manButton.setEnabled(false); //intially were in manual mode so we disbale this button
		manButton.setActionCommand(c.MANBUTTON);
		manButton.addActionListener(c);
		
		//Atomatic Button---------------------------------
		autoButton =  new JButton("Set to Automatic mode");
		autoButton.setActionCommand(c.AUTOBUTTON);
		autoButton.addActionListener(c);
		
		//Return Button----------------------------------
		retButton =  new JButton("Return to original Position");
		retButton.setActionCommand(c.RETBUTTON);
		retButton.addActionListener(c);

		
		
		
		//add buttons to the panel
		myPanel.add(donutButton);
		myPanel.add(locationButton);
		myPanel.add(manButton);
		myPanel.add(autoButton);
		myPanel.add(retButton);
		
		//add the panel to the frame
		test.getContentPane().add(myPanel);
		

		//test.pack();
		test.setVisible(true);
	
	}
	
	//Pre:Need to dispaly were in manual mode
	//Post: THe view has been updated to display were in manual mode
	public void dispManualMode(){
		manButton.setEnabled(false);
		autoButton.setEnabled(true);
	}
	
	//Pre:Need to display to the user that we are in automatic mode
	//Post: The view has been updated to dispaly in automatic mode
	public void dispAutoMode(){
		manButton.setEnabled(true);
		autoButton.setEnabled(false);
	}

	//Pre: The up button has been pressed
	//Post:controler has been updated on the button press
	AbstractAction upPressedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.upPressed();

		}
	};
	
	//Pre: The up button has been released
	//Post:controler has been updated on button release
	AbstractAction upReleasedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.upReleased();

		}
	};
	
	//Pre: The down button has been pressed
	//Post:controler has been updated on the button press
	AbstractAction downPressedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.downPressed();

		}
	};
	
	//Pre: The down button has beeen released
	//Post:controler has been updated on button release
	AbstractAction downReleasedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.downReleased();

		}
	};
	
	//Pre: The left button has been pressed
	//Post:controler has been updated on the button press
	AbstractAction leftPressedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.leftPressed();

		}
	};
	
	//Pre: The left button has been released
	//Post:controler has been updated on button release
	AbstractAction leftReleasedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.leftReleased();

		}
	};
	
	//Pre:The right button has been presed
	//Post:controler has been updated on the button press
	AbstractAction rightPressedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.rightPressed();
		}
	};
	
	//Pre: The right button has been released
	//Post:controler has been updated on button release
	AbstractAction rightReleasedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.rightReleased();

		}
	};
	
	//Pre:The escape button has been pressed
	//Post:controler has been updated on the button press
	AbstractAction escapePressedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.escapePressed();

		}
	};
	
	//Pre:  the escape button has been released
	//Post: controler has been updated on button release
	AbstractAction escapeReleasedAction = new AbstractAction(){
		public void actionPerformed(ActionEvent e) {
			control.escapeReleased();

		}
	};
	
}
