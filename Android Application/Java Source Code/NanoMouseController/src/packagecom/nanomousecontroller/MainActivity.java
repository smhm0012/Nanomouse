package packagecom.nanomousecontroller;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
/**
  *\class MainActivity
  * 
  *\brief This where you can connect and control the mouse
  * 
  * The screen will show a connect button the will go to the ListActivity
  * and four arrow button that will help control the direction of the 
  * Nano-Mouse
  */
public class MainActivity extends Activity {

	Button btn2; 
	String theDevice, myDevice;
	BluetoothDevice Device;
	BluetoothAdapter BTAdapter;
	String[] stringArray;
	Intent intent;
	private static final int CONNECT_DEVICE = 1;
	private BluetoothSocket mainSocket = null;
	OutputStream strOUT = null;
    InputStream strIN = null;
    String leftMotor, rightMotor;
	private UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		btn2 = (Button)findViewById(R.id.button2);
	    btn2.setOnClickListener(new OnClickListener(){
	      public void onClick(View v) {
	    	  
	    	 /* if(mainSocket.isConnected())
	    	  {
	    		  try{
	    		   leftMotor = "0";
	    		   rightMotor = "0";
	    		   NanoMouseResponse(leftMotor,rightMotor);  
				   mainSocket.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	    	  }*/
	    	  Intent go = new Intent(getApplicationContext(),ListActivity.class);
		      startActivity(go);
	      }
	    });
	    intent = getIntent();
	    BTAdapter = BluetoothAdapter.getDefaultAdapter();
	}
     
	 @Override
	  protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	    if (data == null) {return;}
	    Bundle extras = getIntent().getExtras();
	    //theDevice = intent.getStringExtra("Bluetooth");
	    if(extras != null)
	    {
	     theDevice = extras.getString("Bluetooth");
	     Log.d("tag", theDevice);
	     Toast.makeText(getApplicationContext(), theDevice,Toast.LENGTH_LONG).show();
	     stringArray = theDevice.split(" ");
	     myDevice = stringArray[1];  
	    }
	    /*switch (requestCode) 
        {
          case CONNECT_DEVICE:
            if (resultCode == Activity.RESULT_OK) 
            {*/
	          Device = BTAdapter.getRemoteDevice(myDevice);
	          Log.d("connect", "Connecting to ... " + Device);
	          try {
				ConnectionTime();
			  } 
	          catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			  }
	       /* }
            break;
        } */  
    }
	
	 /**
	  * @param v first parameter
	  * 
	  */
	public void Directions(View v)
	{
		/* Gets the id of the button to determine the direction */
		switch(v.getId())
		{
		  case R.id.upArrow:
			  leftMotor = "100";
			  rightMotor = "100";
			  NanoMouseResponse(leftMotor,rightMotor);
			break;
			
		  case R.id.leftArrow:
			  leftMotor = "0";
			  rightMotor = "100";
			  NanoMouseResponse(leftMotor,rightMotor);
			  break;
			  
		  case R.id.rightArrow:
			  leftMotor = "100";
			  rightMotor = "0";
			  NanoMouseResponse(leftMotor,rightMotor);
			  break;
			  
		  case R.id.downArrow:
			  leftMotor = "-100";
			  rightMotor = "-100";
			  NanoMouseResponse(leftMotor,rightMotor);
			  break;
			  
		 default:
			 leftMotor = "100";
			 rightMotor = "100";
			 NanoMouseResponse(leftMotor,rightMotor);
			 
		}
	}
	
	/*
	 * @brief This is where the App listens for the device to Connect 
	 */
	public void ConnectionTime() throws IOException
	{
		BTAdapter.cancelDiscovery();
        try 
        {
          mainSocket = Device.createRfcommSocketToServiceRecord(myUUID);
          mainSocket.connect();
          
          strOUT = mainSocket.getOutputStream();
          strIN = mainSocket.getInputStream();
        }
        catch(IOException ex)
        {
        	mainSocket.close();
        	
        }
	}
	
	/**
	 * @param leftMotor first parameter
	 * @param rightMotor second parameter
	 * 
	 * This where the values that were received from the user
	 * press the button will be sent to the Nano-Mouse
	 */
	public void NanoMouseResponse(String leftMotor,String rightMotor)
	{
		byte[] chat;
		chat = new byte[2];
		chat = leftMotor.getBytes();
		chat = rightMotor.getBytes();
		
		try{
			strOUT.write(chat);
        }
        catch(IOException e){
            e.printStackTrace();
        }
		
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
