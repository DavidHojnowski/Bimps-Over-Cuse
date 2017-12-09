
public class CarController {
	//the commands we are using in the protocol 
	private final static String FORWARD = "FWD";
	private final static String BACKWARD ="BCK";
	private final static String LEFT = "LFT";
	private final static String RIGHT = "RGT";
	private final static String BRAKE = "BRK";
	private final static String DODONUTS = "DNT";
	private final static String LOCATION = "LOC";
	private final static String PAUSE = "PSE";
	private final static String MANUAL = "MAN";
	private final static String AUTOMATIC = "ATM";
	private final static String RETURN = "RET";
	
	private Model carModel;
	private CarSocket carSock;
	
	public CarController(Model carModel){
		this.carModel = carModel;
		carSock = new CarSocket();
		//COMENT OUR FOR TESTING WITHOUT CAR
		carSock.connect();
	}
	//Pre:A key has been pressed need to update the model 
	//Post: The model has been uodated based on the key that has been pressed
	public void keyPressed(Keys key){
		switch(key){
			case UPARROW:
				carModel.goFwd();
				sendMoveCommand();
				break;
			case DOWNARROW:
				carModel.goBckWd();
				sendMoveCommand();
				break;
			case LEFTARROW:
				carModel.goLeft();
				sendMoveCommand();
				break;
			case RIGHTARROW:
				carModel.goRight();
				sendMoveCommand();
				break;
			case ESCAPE:
				//pause
				writeToCar(PAUSE);
				System.out.println("Paused");
				break;
			
		}		

	}
	//Pre:A key has been released need to update the model
	//Post: The model has been updated based on the key relased
	public void keyReleased(Keys key){
		switch(key){
			case UPARROW:
				carModel.stopFwd();
				sendMoveCommand();
				break;
			case DOWNARROW:
				carModel.stopBckWd();
				sendMoveCommand();
				break;
			case LEFTARROW:
				carModel.stopLeft();
				sendMoveCommand();
				break;
			case RIGHTARROW:
				carModel.stopRight();
				sendMoveCommand();
				break;
			default:
				//do nothing
		}

	}
	//Pre: Send one of the commands to move
	//Post:One of the moving commands has been sent based on the sate of the car in the model
	private void sendMoveCommand(){
		/*System.out.println("Is moving "+ carModel.isMoving());
		System.out.println("Going Forward " + carModel.goingFwd());
		System.out.println("Going Backward " + carModel.goingBckWd());
		System.out.println("Going Left "+carModel.goingLeft());
		System.out.println("Going Right "+carModel.goingRight());*/
		if(carModel.inManual()){
			if(carModel.isMoving()){
				//then see what direction were moving
				if(carModel.goingFwd()){
					//send command forward 
					System.out.println("FWD");
					writeToCar(FORWARD);
				}
				else if(carModel.goingBckWd()){
					//send command backward 
					System.out.println("BCK");
					writeToCar(BACKWARD);
				}
				else if(carModel.goingLeft()){
					//send command going left
					System.out.println("LEFT");
					writeToCar(LEFT);
				}
				else if(carModel.goingRight()){
					//send command going right
					System.out.println("RIGHT");
					writeToCar(RIGHT);
				}
			}
			else{//We're not moving
				//may want to send break command
				System.out.println("BRAKE");
				writeToCar(BRAKE);
			}
		}
	}
	
	//Pre:need to send the do donuts command
	//Post:The do donuts command has beeen sent
	public void doDonuts(){
		writeToCar(DODONUTS);
		System.out.println("Do Donuts");
	}
	
	//Pre:Need to send the location command
	//Post: The location command has been sent
	public void location(){
		writeToCar(LOCATION);
		System.out.println("Get Current location");
		// read from socket
		
		String location = "";
		//COMENT OUT FOR TESTING WITHOUT CAR
		location = carSock.read(); //blocking read call
		System.out.println("Current location is "+ location);
		
	}
	
	//Pre:Need to send the manual command
	//Post:The manual command has been sent
	public void setManual(){
		writeToCar(MANUAL);
		System.out.println("Manual Mode");
	}
	
	//Pre:need to send the Automatic command
	//Post:the automatic command has been sent
	public void setAutomatic(){
		writeToCar(AUTOMATIC);
		System.out.println("Automatic Mode");
	}
	
	//Pre:Need to send the return comand
	//Post:The return comand has beeen sent
	public void returnBack(){
		writeToCar(RETURN);
		System.out.println("Return back to last location");
	}
	
	//Pre: Need to write some data s to the car
	//Post: S has been sent to the car using the car socket
	private void writeToCar(String s){	
		//COMENT OUR FOR TESTING WITHOUT CAR
		carSock.write(s);
		
	}
}

