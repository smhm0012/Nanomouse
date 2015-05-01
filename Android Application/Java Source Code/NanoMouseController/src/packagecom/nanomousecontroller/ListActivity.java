package packagecom.nanomousecontroller;

import java.util.Set;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;

/**
 * @class ListActivity
 * @brief A screen that displays the paired device in a list view
 */
public class ListActivity extends Activity {

	private BluetoothAdapter BTAdapter = null;
	ListAdapter myArray;
	String Blue;
	private ArrayAdapter<String> Devices;
	ListView listV;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_list);
		listV = (ListView)findViewById(R.id.DeviceList);
		BTAdapter = BluetoothAdapter.getDefaultAdapter();
		/** 
		 * if the bluetooth isn't on then the user will 
		 * a notification asking to turn it on
		 */
		if(!BTAdapter.isEnabled())
		{
			Intent BTEnabled = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(BTEnabled, 0);
		}
		BTDeviceSearch();
		
		/* When one of the pairied device gets selected then it gets sent to the MainActivity */
		listV.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> Devices,View v, int position, long id){
				String item = (String) listV.getItemAtPosition(position);
				Intent SendBack = new Intent(ListActivity.this,MainActivity.class);
				Log.d("msg", item);
				SendBack.putExtra(item,"Bluetooth");
				setResult(Activity.RESULT_OK, SendBack);
				startActivity(SendBack);
				//finish();
			}
		});
	}
	
	/**
	 * Displays the paired device in a list view
	 */
	public void BTDeviceSearch()
	{
		Set<BluetoothDevice> pairedDevices = BTAdapter.getBondedDevices();
		Devices = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
		if (pairedDevices.size() > 0) {

		    for (BluetoothDevice device : pairedDevices) 
		    {
		        Devices.add(device.getName()+"\t"+device.getAddress());
		    }
		    listV.setAdapter(Devices);
		}
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.list, menu);
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
