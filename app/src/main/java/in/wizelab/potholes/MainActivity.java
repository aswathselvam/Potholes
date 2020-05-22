package in.wizelab.potholes;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;


public class MainActivity extends AppCompatActivity implements SensorEventListener, LocationListener {
    private SensorManager sensorManager;
    private Sensor sensor;

    //sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
    //sensor = sensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);

    protected LocationManager locationManager;
    protected LocationListener locationListener;
    protected Context context;
    double lat,lon, speed;
    String provider;
    protected boolean gps_enabled, network_enabled;

    Socket socket = null;
    OutputStream out = null;
    PrintWriter output = null;

    int dataPoint =0, calibCount=0,WINDOW_SIZE=10;
    float[] x=new float[WINDOW_SIZE],y=new float[WINDOW_SIZE],z=new float[WINDOW_SIZE];
    float[][] Rotation;
    float sumx,sumy,sumz,sdx,sdy,sdz,soqx,soqy,soqz,maxx,maxy, maxz, minx, miny, minz;
    float offsetx,offsety,offsetz,magoffset,unitoffsetx,unitoffsety,unitoffsetz;
    boolean calibrated=false;
    boolean accelTriggered =false;
    long time;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(this, sensor, sensorManager.SENSOR_DELAY_GAME);

        locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, this);

        for(int i=0;i<WINDOW_SIZE;i++){
            x[i]=y[i]=z[i]=0;
        }
        sumx=sumy=sumz=sdx=sdy=sdz=0;
        //WebView webView =(WebView) findViewById(R.id.webView);
        //webView.setWebViewClient(new WebViewClient());
        //webView.setWebChromeClient(new WebChromeClient());
        //webView.setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
        //webView.getSettings().setLoadsImagesAutomatically(true);
        //webView.getSettings().setJavaScriptEnabled(true);
        //webView.getSettings().setCacheMode(WebSettings.LOAD_DEFAULT);
        //webView.getSettings().setBuiltInZoomControls(true);
        //webView.loadUrl("http://google.com");
        //webView.loadUrl("http://localhost:1880/worldmap/");
    }
    boolean enable_detection=true;
    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        //Log.d("MainActivity", "X:"+sensorEvent.values[0]);

        //sensorManager.getRotationMatrixFromVector();
        Log.d("MainActivity","RR:"+(System.currentTimeMillis()-time));
        if(System.currentTimeMillis()-time<50){
            //return;
        }
        time=System.currentTimeMillis();
        if(!enable_detection){
            return;
        }
        if(calibrated){
            x[dataPoint]=sensorEvent.values[0]-offsetx;
            y[dataPoint]=sensorEvent.values[1]-offsety;
            z[dataPoint]=sensorEvent.values[2]-offsetz;
            for(int j=0;j<3;j++){
                x[dataPoint]+=x[dataPoint]*Rotation[0][j];
            }
            for(int j=0;j<3;j++){
                y[dataPoint]+=x[dataPoint]*Rotation[1][j];
            }
            for(int j=0;j<3;j++) {
                z[dataPoint] += z[dataPoint]*Rotation[2][j];
            }
            Log.d("MainActivity","x:" +x[dataPoint]+" y:"+y[dataPoint]+" z:"+z[dataPoint]);
            dataPoint =(dataPoint +1);
            dataPoint = dataPoint % WINDOW_SIZE;
            sumx=sumy=sumz=0;
            for(int j=0;j<WINDOW_SIZE;j++){
                sumx+=x[j];
                sumy+=y[j];
                sumz+=z[j];
                soqx+=x[j]*x[j];
                soqy+=x[j]*x[j];
                soqz+=x[j]*x[j];
            }
            sdx=(soqx/WINDOW_SIZE)-(sumx/WINDOW_SIZE);
            sdy=(soqy/WINDOW_SIZE)-(sumy/WINDOW_SIZE);
            sdz=(soqz/WINDOW_SIZE)-(sumz/WINDOW_SIZE);
            Log.d("MainActivity","sdz"+sdx+" sdy"+sdy+" sdz"+sdz);
        }else{
            sumx+=sensorEvent.values[0];
            sumy+=sensorEvent.values[0];
            sumz+=sensorEvent.values[0];
            calibCount+=1;
            if(calibCount>=10){
                offsetx=sumx/10;
                offsetx=sumy/10;
                offsetx=sumz/10;
                magoffset=(float)Math.sqrt(offsetx*offsetx+offsety*offsety+offsetz*offsetz);
                unitoffsetx=offsetx/magoffset;
                unitoffsety=offsety/magoffset;
                unitoffsetz=offsetz/magoffset;
                float ax,ay,az,bx,by,bz,cx,cy,cz;
                ax= (float) (3.26/9.8);ay=(float)(0.06/9.8);az=(float)(9.15/9.8);
                Rotation=new float[][]{{unitoffsetx*ax, unitoffsetx*ay, unitoffsetx*az},
                        {unitoffsety*ax, unitoffsety*ay, unitoffsety*az},
                        {unitoffsetz*ax, unitoffsetz*ay, unitoffsetz*az}};
                calibrated=true;
            }
        }

        if(accelTriggered) {
            Log.d("MainActivity", "X:"+sensorEvent.values[0]);

            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try  {
                        socket = new Socket("aswath8-35601.portmap.io", 35601);
                        out = socket.getOutputStream();
                        output = new PrintWriter(out,true);
                        double w=10.1,l=12.2,d=4.4;
                        String url="\""+"https://i.imgur.com/kflMpiN.png"+"\"";
                        output.println("{\"lat\":"+lat+",\"lon\":"+lon+",\"width\":"+w+",\"len\":"+l+",\"depth\":"+d+",\"url\":"+url+"}");
                        socket.close();
                        //Toast.makeText(getApplicationContext(), "Pothole Reported", Toast.LENGTH_LONG).show();
                        Log.d("Socket", "success");
                        requestRefresh();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
            thread.start();
            }
        }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    @Override
    public void onLocationChanged(Location location) {
        lat= location.getLatitude();
        lon=location.getLongitude();
        speed=location.getSpeed();
        Toast.makeText(getApplicationContext(), "Loctation Updated", Toast.LENGTH_LONG).show();
        requestRefresh();
        Log.d("MainActivity","Latitude:" + location.getLatitude() + ", Longitude:" + location.getLongitude());
    }

    @Override
    public void onStatusChanged(String s, int i, Bundle bundle) {

    }

    @Override
    public void onProviderEnabled(String s) {

    }

    @Override
    public void onProviderDisabled(String s) {

    }

    public void requestRefresh(){
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                Log.d("REQUEST", "Trying...");
                try  {
                    socket = new Socket("127.0.0.1", 2021);
                    out = socket.getOutputStream();
                    output = new PrintWriter(out,true);
                    output.println("{\"request\":"+"\"true\""+",\"lat\":"+lat+",\"lon\":"+lon+"}");
                    socket.close();
                    Log.d("REQUEST", "success");
                } catch (Exception e) {
                    e.printStackTrace();
                    Log.d("REQUEST", "failure");
                }
            }
        });
        thread.start();
    }
}
