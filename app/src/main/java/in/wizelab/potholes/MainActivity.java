package in.wizelab.potholes;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.AndroidException;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;


public class MainActivity extends AppCompatActivity implements SensorEventListener, LocationListener {
    private SensorManager sensorManager;
    private Sensor mAccelsensor, mGyrosensor;
    String sensorName;

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

    int dataPoint =0, calibCount=0,WINDOW_SIZE=50;
    float[] x=new float[WINDOW_SIZE],y=new float[WINDOW_SIZE],z=new float[WINDOW_SIZE];
    float[] xg=new float[WINDOW_SIZE],yg=new float[WINDOW_SIZE],zg=new float[WINDOW_SIZE];
    float[][] Rotation;
    float sumx,sumy,sumz,sdx,sdy,sdz,soqx,soqy,soqz,varx,vary,varz,maxx,maxy, maxz, minx, miny, minz,meanx,meany, meanz;
    float sumxg,sumyg,sumzg,sdxg,sdyg,sdzg,soqxg,soqyg,soqzg,varxg,varyg,varzg,maxxg,maxyg, maxzg, minxg, minyg, minzg,meanxg,meanyg, meanzg;

    String effectOfRiding="None";
    float offsetx,offsety,offsetz,magoffset,unitoffsetx,unitoffsety,unitoffsetz;
    double prediction;
    boolean calibrated=true;
    boolean accelTriggered =false;
    long time;

    TextView tvCount;
    int count=-1;
    static {
        System.loadLibrary("main");
    }
    public native double doubleFromJNI(double[] x);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvCount=(TextView)findViewById(R.id.tvDetected);

        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mAccelsensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        mGyrosensor = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
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
        time=System.currentTimeMillis()+1000;
        sumx=sumy=sumz=sdx=sdy=sdz=0;
        //double[] arr={-0.2114  , -2.6086 ,  10.0036  ,  1.1884  ,  0.9656  ,  3.5812   , 8.4569 ,  -9.9138  , -1.8262  ,  5.5666  , 13.1518  ,  1.8972};
        double[] arr={-0.8464  , -1.7438   , 9.6011 ,   0.1285   , 0.2934 ,   0.3908 ,  -0.6621 ,  -0.1646  ,  0.4073  ,  0.3701   , 0.7951   , 0.1004};

        prediction=doubleFromJNI(arr);
        //Log.d("MainActivity",String.valueOf(prediction));
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

    protected void onResume() {
        super.onResume();
        sensorManager.registerListener(this, mAccelsensor, sensorManager.SENSOR_DELAY_GAME);
        sensorManager.registerListener(this, mGyrosensor, SensorManager.SENSOR_DELAY_GAME);
    }


    boolean enable_detection=true, potholeDetected=false;
    float diffx=0,diffy=0,diffz=0;
    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        //Log.d("MainActivity", "X:"+sensorEvent.values[0]);

        //sensorManager.getRotationMatrixFromVector();
        //Log.d("MainActivity","RR:"+(System.currentTimeMillis()-time));

        sensorName = sensorEvent.sensor.getName();

        dataPoint =(dataPoint +1);
        dataPoint = dataPoint % WINDOW_SIZE;
        //Log.d("MainActivity",sensorName+ ": X: " + sensorEvent.values[0] + "; Y: " + sensorEvent.values[1] + "; Z: " + sensorEvent.values[2] + ";");
        if(sensorName.contentEquals("LSM6DS3 Gyroscope")){
            xg[dataPoint]=sensorEvent.values[0];
            yg[dataPoint]=sensorEvent.values[1];
            zg[dataPoint]=sensorEvent.values[2];
        }else{
            x[dataPoint]=sensorEvent.values[0];
            y[dataPoint]=sensorEvent.values[1];
            z[dataPoint]=sensorEvent.values[2];
        }
        if(System.currentTimeMillis()-time<15 ){
            return;
        }
        time=System.currentTimeMillis();

        if(!enable_detection){
            return;
        }
        if(calibrated){

            /*
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
            */

            sumx=sumy=sumz=soqx=soqy=soqz=0;
            for(int j=0;j<WINDOW_SIZE;j++){
                sumx+=x[j];
                sumy+=y[j];
                sumz+=z[j];
                soqx+=x[j]*x[j];
                soqy+=y[j]*y[j];
                soqz+=z[j]*z[j];
            }
            meanx=sumx/WINDOW_SIZE;
            meany=sumy/WINDOW_SIZE;
            meanz=sumz/WINDOW_SIZE;
            varx=(soqx/WINDOW_SIZE)-(meanx*meanx);
            vary=(soqy/WINDOW_SIZE)-(meany*meany);
            varz=(soqz/WINDOW_SIZE)-(meanz*meanz);
            varx=Math.abs(varx);
            vary=Math.abs(vary);
            varz=Math.abs(varz);
            sdx=(float)Math.sqrt(varx);
            sdy=(float)Math.sqrt(vary);
            sdz=(float)Math.sqrt(varz);

            sumxg=sumyg=sumzg=soqxg=soqyg=soqzg=0;
            for(int j=0;j<WINDOW_SIZE;j++){
                sumxg+=xg[j];
                sumyg+=yg[j];
                sumzg+=zg[j];
                soqxg+=xg[j]*xg[j];
                soqyg+=yg[j]*yg[j];
                soqzg+=zg[j]*zg[j];
            }
            meanxg=sumxg/WINDOW_SIZE;
            meanyg=sumyg/WINDOW_SIZE;
            meanzg=sumzg/WINDOW_SIZE;
            /*
            diffx=diffy=diffz=0;
            for( int j=0;j<WINDOW_SIZE;j++){
                diffx=xg[i]-;
                diffy=;
                diffz=;
            }
            */
            varxg=(soqxg/WINDOW_SIZE)-(meanxg*meanxg);
            varyg=(soqyg/WINDOW_SIZE)-(meanyg*meanyg);
            varzg=(soqzg/WINDOW_SIZE)-(meanzg*meanzg);
            varxg=Math.abs(varxg);
            varyg=Math.abs(varyg);
            varzg=Math.abs(varzg);

            //Log.d("Variance:",varx+":"+vary+":"+varz+":");
            sdxg=(float)Math.sqrt(varxg);
            sdyg=(float)Math.sqrt(varyg);
            sdzg=(float)Math.sqrt(varzg);
            double[] arr={(double)meanx,(double)meany,(double)meanz,(double)sdx,(double)sdy,(double)sdz,
                    (double)meanxg,(double)meanyg,(double)meanzg,(double)sdxg,(double)sdyg,(double)sdzg,
            };
            //Log.d("Input",arr[0]+" "+arr[1]+" "+arr[2]+" "+arr[3]+" "+arr[4]+" "+arr[5]+" "+arr[6]+" "
             //    + arr[7]+" "+arr[8]+" "+arr[9]+" "+arr[10]+" "+arr[11]);

            prediction=doubleFromJNI(arr);

            if (potholeDetected && prediction==1){
                potholeDetected=false;
                tvCount.setTextColor(Color.GRAY);
            }
            if(prediction==-1 && !potholeDetected){
                tvCount.setTextColor(Color.RED);
                //Schmidt trigger
                potholeDetected=true;
                //Timeout 20seconds
                time=System.currentTimeMillis()+2000;
                count++;
                if(sdz>4.8){
                    effectOfRiding="High";
                }else if(sdz>2.8){
                    effectOfRiding="Moderate";
                }else if(sdz>1.8){
                    effectOfRiding="Low";
                }else{
                    effectOfRiding="Very Low";
                }
                effectOfRiding="\""+effectOfRiding+"\"";
                accelTriggered=true;
                tvCount.setText(String.valueOf(count));
            }
            Log.d("MainActivity","Pred: "+prediction);
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
            accelTriggered=false;
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try  {
                        socket = new Socket("aswath8-35601.portmap.io", 35601);
                        out = socket.getOutputStream();
                        output = new PrintWriter(out,true);
                        double w=10.1,l=12.2,d=4.4;
                        String url="\""+"https://i.imgur.com/kflMpiN.png"+"\"";
                        output.println("{\"lat\":"+lat+",\"lon\":"+lon+",\"width\":"+w+",\"len\":"+l
                                +",\"num\":"+1+",\"area\":"+0+",\"effect\":"+effectOfRiding+",\"url\":"+url+"}");
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
