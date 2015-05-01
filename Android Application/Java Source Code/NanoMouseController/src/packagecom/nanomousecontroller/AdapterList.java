package packagecom.nanomousecontroller;

import java.util.ArrayList;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;

public class AdapterList extends ArrayAdapter<String> {
  private final Context context;
  private final int textViewResourceId;

  public AdapterList(Context context, int textViewResourceId, ArrayList<String> objects) 
  {
	super(context,textViewResourceId,objects);
    this.context = context;
    this.textViewResourceId = textViewResourceId;
  }
  @Override
  public View getView(int position, View convertView, ViewGroup parent) 
  {
	 LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	 View rowView = inflater.inflate(textViewResourceId, parent, false);

	        return rowView;
	    }

}
