
public class CarController {
	//the commands we are using in the protocol 
	private final static String FORWARD = "FWD";
	private final static String BACKWARD ="BCK";
	private final static String LEFT = "LFT";
	private final static String RIGHT = "RGT";
	private final static String STOP = "STP";
	private final static String BRAKE = "BRK";
	
	Model carModel;
	CarSocket carSock;
	
	public CarController(Model carModel){
		this.carModel = carModel;
		carSock = new CarSocket();
		carSock.connect();
	}
	
	public void keyPressed(Keys key){
		switch(key){
			case UPARROW:
				carModel.goFwd();
				break;
			case DOWNARROW:
				carModel.goBckWd();
				break;
			case LEFTARROW:
				carModel.goLeft();
				break;
			case RIGHTARROW:
				carModel.goRight();
				break;
		}		
		sendCommand();
	}
	
	public void keyReleased(Keys key){
		switch(key){
			case UPARROW:
				carModel.stopFwd();
				break;
			case DOWNARROW:
				carModel.stopBckWd();
				break;
			case LEFTARROW:
				carModel.stopLeft();
				break;
			case RIGHTARROW:
				carModel.stopRight();
				break;
		}
		sendCommand();
	}
	
	private void sendCommand(){
		/*System.out.println("Is moving "+ carModel.isMoving());
		System.out.println("Going Forward " + carModel.goingFwd());
		System.out.println("Going Backward " + carModel.goingBckWd());
		System.out.println("Going Left "+carModel.goingLeft());
		System.out.println("Going Right "+carModel.goingRight());*/
		
		if(carModel.isMoving()){
			//then see what direction were moving
			if(carModel.goingFwd()){
				if(carModel.goingLeft()){ //NOT CURRENTLY USED
					//send command forward left
					System.out.println("FWD + LEFT");
					writeToCar("FWD + LEFT");
				}
				else if(carModel.goingRight()){ //NOT CURRENTLY USED
					//send command forward right
					System.out.println("FWD + RIGHT");
					writeToCar("FWD + RIGHT");
				}
				else{
					//send command forward 
					System.out.println("FWD");
					writeToCar(FORWARD);
				}
			}
			else if(carModel.goingBckWd()){
				if(carModel.goingLeft()){//NOT CURRENTLY USED
					//send command backward left
					System.out.println("BCKWD + LEFT");
					writeToCar("BCKWD + LEFT");
				}
				else if(carModel.goingRight()){//NOT CURRENTLY USED
					//send command backward right
					System.out.println("BCKWD + RIGHT");
					writeToCar("BCKWD + RIGHT");
				}
				else{
					//send command backward 
					System.out.println("BCK");
					writeToCar(BACKWARD);
				}
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
	
	
	private void writeToCar(String s){		
		carSock.write(s);
		
	}
}

