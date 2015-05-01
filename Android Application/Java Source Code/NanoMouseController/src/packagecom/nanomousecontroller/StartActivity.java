package packagecom.nanomousecontroller;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
/**
 * @mainpage Nano-Mouse Application
 * @details This is another version of the Nano-Mouse Controller App
 * \nwhich uses java code in Eclipse Juno
 */

/**
 * @author Jason Nixon
 * @file StartActivity.java 
 * @details Basic start screen to get into the Main part of the 
 * \n application
 */

public class StartActivity extends Activity {

	Button btn1;
	Intent go;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_start);
		btn1 = (Button)findViewById(R.id.button1);
		
		// When the Button on the Start Screen is pressed it will go to the Main Screen
	    btn1.setOnClickListener(new OnClickListener(){
	      public void onClick(View v) {
	    	  go = new Intent(getApplicationContext(),MainActivity.class);
		      startActivity(go);
	      }
	      });
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.start, menu);
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
