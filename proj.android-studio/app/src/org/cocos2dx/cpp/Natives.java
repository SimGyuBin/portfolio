package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.regex.Pattern;

import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONObject;

import com.facebook.AccessToken;
import com.facebook.GraphRequest;
import com.facebook.GraphRequest.GraphJSONObjectCallback;
import com.facebook.GraphResponse;
import com.facebook.HttpMethod;
import com.facebook.login.LoginManager;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.ShareDialog;
import com.onesignal.OneSignal;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.ActivityNotFoundException;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.StatFs;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.widget.Toast;

class ImageModel {
    public String path = null;
    public String date_modified = null;
    public String date_added = null;
    public String fulldate = null;
    public String size = null;
    public String width = null;
    public String height = null;
    public String category_name = null;
    public String category_path = null;
}

class PaymentScheme {

    public final static String ISP = "ispmobile";
    public final static String BANKPAY = "kftc-bankpay";
    public final static String HYUNDAI_APPCARD = "hdcardappcardansimclick"; //intent:hdcardappcardansimclick://appcard?acctid=201605092050048514902797477441#Intent;package=com.hyundaicard.appcard;end;
    public final static String KB_APPCARD = "kb-acp"; //intent://pay?srCode=5681318&kb-acp://#Intent;scheme=kb-acp;package=com.kbcard.cxh.appcard;end;

    public final static String PACKAGE_ISP = "kvp.jjy.MispAndroid320";
    public final static String PACKAGE_BANKPAY = "com.kftc.bankpay.android";

}

public class Natives {

    private static Natives delegate;
    public AppActivity mainActivity;
    public Handler handler;
    public float gapWidth, gapHeight;
    public float displayRatio;
    public boolean mbln_IntroFinish = false;

    public static final int READ_EXTERNAL_STORAGE_REQUEST = 3000;


    static public void setActivity(AppActivity activity) {
        Natives.sharedDelegate().mainActivity = activity;
        Natives.sharedDelegate().handler = new Handler();
    }

    public static Natives sharedDelegate() {
        if (delegate == null) {
            delegate = new Natives();
        }

        return delegate;
    }

    private Natives() {
        this.mainActivity = null;
        this.handler = null;
    }

    /***************************** Java -> C++ ****************************/

    public static native void nativeCallBackPathOfAllImages(final String path, final String date_modified, final String date_added,
                                                            final String fulldate, final String size, final String width, final String height, final String category_path, final String category_name);

    public static native void nativeCallBackPathOfAllImagesComplete();

    public static native void nativeCallBackGetThumbnailImage(final byte[] byteArray, final int length,
                                                              final String path, final String width, final String height);

    public static native void nativeCallBackGetFullScreenImage(final byte[] byteArray, final int length,
                                                               final String path, final String width, final String height);

    public static native void nativeCallBackGetUploadImage(final byte[] byteArray, final int length, final String path,
                                                           final String width, final String height, final String date);

    public static native void nativeCallBackKeyCodeBack();

    public static native void nativeCallBackFacebookLogin(final boolean isLogin ,final String msg);
    public static native void nativeCallBackFacebookLoginCheck(final boolean isLogin);
    public static native void nativeCallBackFacebookMyInfo(final String data);
    public static native void nativeCallBackFacebookUserAlbums(final String data);
    public static native void nativeCallBackFacebookUserImages(final String albumID, final String data);
    public static native void nativeCallBackPurchaseLayerloadURL(final String url);
    public static native void nativeCallBackGoogleSignInSuccess(final String displayName, final String Id, final String Email, final String token);
    public static native void nativeCallBackKakaoLoginSuccess(final String userId, final String nickName, final String email);
    public static native void nativeCallBackIdsAvailable(final String userId, final String registrationId);
    public static native void nativeCallBackOneSignalHandleNotification(final String strMessage, final String strTitle, final String strPush_event, final boolean blnActive);
    public static native void successSaveImage();
    public static native void canceledImagePicker();


    public static void glthread_PathOfAllImages(final String path, final String date_modified, final String date_added,
                                                final String fulldate, final String size, final String width, final String height, final String category_path, final String category_name) {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackPathOfAllImages(path, date_modified, date_added, fulldate, size, width, height, category_path, category_name);
            }
        });
    }

    public static void glthread_PathOfAllImagesComplete() {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackPathOfAllImagesComplete();
            }
        });
    }

    public static void glthread_GetThumbnailImage(final byte[] byteArray, final int length, final String path,
                                                  final String width, final String height) {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackGetThumbnailImage(byteArray, length, path, width, height);
            }
        });
    }

    public static void glthread_GetFullScreenImage(final byte[] byteArray, final int length, final String path,
                                                   final String width, final String height) {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackGetFullScreenImage(byteArray, length, path, width, height);
            }
        });
    }

    public static void glthread_GetUploadImage(final byte[] byteArray, final int length, final String path,
                                               final String width, final String height, final String date) {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackGetUploadImage(byteArray, length, path, width, height, date);
            }
        });
    }

    public static void glthread_KeyCodeBack() {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackKeyCodeBack();
            }
        });
    }

    public static void glthread_FacebookLogin(final boolean isLogin ,final String msg) {

        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackFacebookLogin(isLogin, msg);
            }
        });
    }

    public static void glthread_FacebookLoginCheck(final boolean isLogin) {

        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackFacebookLoginCheck(isLogin);
            }
        });
    }

    public static void glthread_FacebookMyInfo(final String data) {

        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackFacebookMyInfo(data);
            }
        });
    }

    public static void glthread_FacebookUserAlbums(final String data) {

        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackFacebookUserAlbums(data);
            }
        });
    }

    public static void glthread_FacebookUserAlbums(final String albumID, final String data) {

        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackFacebookUserImages(albumID, data);
            }
        });
    }

    public static void glthread_SuccessPicture()
    {
        Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg)  {
                Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        Natives.successSaveImage();	// c++로 저장 성공 여부 알리기
                    }
                });
            }
        };
        handler.sendEmptyMessageDelayed(0, 100);
    }

    public static void glthread_CancelPicture()
    {
        Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg)  {
                Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        Natives.canceledImagePicker();	// c++로 저장 실패 여부 알리기
                    }
                });
            }
        };
        handler.sendEmptyMessageDelayed(0, 100);
    }

    public static void glthread_purchaseLayerloadURL(final String url)
    {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackPurchaseLayerloadURL(url);
            }
        });
    }

    public static void glthread_GoogleSignInSuccess(final String displayName, final String Id, final String Email, final String token)
    {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackGoogleSignInSuccess(displayName, Id, Email, token);
            }
        });
    }

    public static void glthread_KakaoLoginSuccess(final String userId, final String nickName, final String email)
    {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackKakaoLoginSuccess(userId, nickName, email);
            }
        });
    }

    public static void glthread_idsAvailable(final String userId, final String registrationId)
    {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackIdsAvailable(userId, registrationId);
            }
        });
    }

    public static void glthread_OneSignalHandleNotification(final String strMessage, final String strTitle, final String strPush_event, final boolean blnActive)
    {
        Natives.sharedDelegate().mainActivity.mGLView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Natives.nativeCallBackOneSignalHandleNotification(strMessage, strTitle, strPush_event, blnActive);
            }
        });
    }

    /***************************** C++ -> Java ****************************/

    // 어플의 라이브러리 폴더 경로
    public static String getLibraryPath()
    {
        return Environment.getExternalStorageDirectory().getAbsolutePath()+ "/Android/data/" + Natives.sharedDelegate().mainActivity.getPackageName()+ "/.Library/";
    }

    public static void saveImage(final boolean visible) {
        String path = Cocos2dxHelper.getCocos2dxWritablePath() + "/" + "imageNameToSave.jpg";
        Log.d("Cocos2dxActivity", "Paht to check --" + path);

        File imgFile = new File(path);

        BitmapFactory.Options option = new BitmapFactory.Options();
        option.inScaled = false;
        option.inDensity = 300;

        Bitmap myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath(), option);
        ArrayList<Uri> uris = new ArrayList<Uri>();
        uris.add(Uri.parse(path));

        OutputStream output;
        // Find the SD Card path
        File filepath = Environment.getExternalStorageDirectory();

        // Create a new folder in SD Card
        File dir = new File(filepath.getAbsolutePath() + "/PixHolicSaveImage");
        dir.mkdirs();

        // Create a name for the saved image
        File file = new File(dir, "imageNameToSave.jpg");

        try {

            output = new FileOutputStream(file);

            // Compress into png format image from 0% - 100%
            myBitmap.compress(Bitmap.CompressFormat.JPEG, 100, output);
            output.flush();
            output.close();
        }

        catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        Intent intent = new Intent();
        Uri pngUri = Uri.fromFile(file);

        intent.setAction(Intent.ACTION_SEND);
        intent.putExtra(Intent.EXTRA_STREAM, pngUri);
        intent.setType("image/jpeg");

        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.startActivity(Intent.createChooser(intent, "Share Image"));
    }

    public static void nativeCallChangeOrientation(int flag) {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.changeOrientation(flag);
    }

    public static void nativeCallPathOfAllImages() {

        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;

        if (ContextCompat.checkSelfPermission(mainActivity,Manifest.permission.READ_EXTERNAL_STORAGE)!= PackageManager.PERMISSION_GRANTED) {

            // 이 권한을 필요한 이유를 설명해야하는가?
            if (ActivityCompat.shouldShowRequestPermissionRationale(mainActivity,Manifest.permission.READ_EXTERNAL_STORAGE)) {

                // 다이어로그같은것을 띄워서 사용자에게 해당 권한이 필요한 이유에 대해 설명합니다
                // 해당 설명이 끝난뒤 requestPermissions()함수를 호출하여 권한허가를 요청해야 합니다

            } else {

                ActivityCompat.requestPermissions(mainActivity,
                        new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                        READ_EXTERNAL_STORAGE_REQUEST);

                // 필요한 권한과 요청 코드를 넣어서 권한허가요청에 대한 결과를 받아야 합니다

            }
        }
        else
        {
            AsyncTask.execute( new Runnable(){
                public void run() {

                    ArrayList<ImageModel> ImageModels = Natives.sharedDelegate().mainActivity.getPathOfAllImages();

                    for (ImageModel model : ImageModels) {

                        glthread_PathOfAllImages(model.path, model.date_modified, model.date_added, model.fulldate,
                                model.size, model.width, model.height, model.category_path, model.category_name);
                    }

                    glthread_PathOfAllImagesComplete();

                }
            });
        }

        int permissionCheck = ContextCompat.checkSelfPermission(mainActivity, Manifest.permission.READ_EXTERNAL_STORAGE);

        if(permissionCheck== PackageManager.PERMISSION_DENIED)
        {

        }
        else
        {
            // 권한 있음
        }
    }

    public static void nativeCallGetThumbnailImage(String strUrl, int width, int height) {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.createThumbnailImage(strUrl, width, height);
    }

    public static void nativeCallGetFullScreenImage(String strUrl, int width, int height) {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.createFullScreenImage(strUrl, width, height);
    }

    public static void nativeCallGetUploadImage(String strUrl) {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.createUploadImage(strUrl);
    }

    public static String nativeCallAES256Encrypt(String plainText, String key) {
        String strPlainText = plainText;
        String strKey = key;
        String encodeText = null;

        // Encrypt
        try {
            encodeText = AES256Cipher.AES_Encode(strPlainText, strKey);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return encodeText;
    }

    public static String nativeCallAES256Decrypt(String base64Text, String key) {
        String strBase64Text = base64Text;
        String strKey = key;
        String decodeText = null;

        // Decrypt
        try {
            decodeText = AES256Cipher.AES_Decode(strBase64Text, strKey);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return decodeText;
    }

    public static void nativeCallClipboard(String paste) {

        final String text = paste;

        new Thread(new Runnable() {
            @Override
            public void run() {
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        if (android.os.Build.VERSION.SDK_INT >= 11) {
                            Natives.setClipboard11(text);
                        } else {
                            Natives.setClipboard9(text);
                        }
                    }
                });
            }
        }).start();
    }

    @SuppressWarnings("deprecation")
    public static void setClipboard9(String text) {
        android.text.ClipboardManager clipboard = (android.text.ClipboardManager) Natives.sharedDelegate().mainActivity
                .getBaseContext().getSystemService(Context.CLIPBOARD_SERVICE);
        clipboard.setText(text);
    }

    @SuppressLint("NewApi")
    public static void setClipboard11(String text) {
        ClipboardManager clipboard = (ClipboardManager) Natives.sharedDelegate().mainActivity.getBaseContext()
                .getSystemService(Context.CLIPBOARD_SERVICE);
        ClipData clip = ClipData.newPlainText("label", text);
        clipboard.setPrimaryClip(clip);
    }

    public static void nativeCallBrowser(String strUrl) {

        final String url = strUrl;

        new Thread(new Runnable() {
            @Override
            public void run() {
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {

                        Intent intent = new Intent(Intent.ACTION_VIEW);

                        Uri uri = Uri.parse(url);

                        intent.setData(uri);

                        Natives.sharedDelegate().mainActivity.startActivity(intent);
                    }
                });
            }
        }).start();
    }

    public static void nativeCallSaveImage(String strFilename, byte[] buffers, int nSize) {

        String path = Cocos2dxHelper.getCocos2dxWritablePath() + "/" + strFilename + ".jpg";

        Bitmap bitmap = BitmapFactory.decodeByteArray( buffers, 0, nSize ) ;
        ArrayList<Uri> uris = new ArrayList<Uri>();
        uris.add(Uri.parse(path));

        OutputStream output;
        // Find the SD Card path
        File filepath = Environment.getExternalStorageDirectory();

        // Create a new folder in SD Card
        File dir = new File(filepath.getAbsolutePath() + "/Pzle");
        dir.mkdirs();

        // Create a name for the saved image
        File file = new File(dir, strFilename + ".jpg");

        try {

            output = new FileOutputStream(file);

            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, output);
            output.flush();
            output.close();
        }
        catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.sendBroadcast(new Intent( Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, Uri.fromFile(file)) );

//		Intent intent = new Intent();
//		Uri pngUri = Uri.fromFile(file);
//
//		intent.setAction(Intent.ACTION_SEND);
//		intent.putExtra(Intent.EXTRA_STREAM, pngUri);
//		intent.setType("image/jpeg");
//
//		AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
//		mainActivity.startActivity(Intent.createChooser(intent, "Share Image"));
    }

    public static void nativeCallKakaoLogin()
    {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.kakaoLogin();
    }

    public static void nativeCallKakaoLogout()
    {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.kakaoLoginOut();
    }

    public static void nativeCallAndroidDestroy()
    {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.onDestroy();
    }

    public static void nativeCallAndroidToast(String msg) {

        final String toastMsg = msg;
        final int duration = 3000;
        new Thread(new Runnable() {
            @Override
            public void run() {
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {

                        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
                        Toast.makeText(mainActivity.getApplicationContext(), toastMsg, duration).show();

                    }
                });
            }
        }).start();
    }

    public static String getDeviceId() {
        return getModelName() + GUIDGenerator.getRandomGUIDString();
    }

    public static String getAppVersionString() {

        AppActivity activity = Natives.sharedDelegate().mainActivity;
        String versionName = "";

        try {
            PackageManager manager = activity.getPackageManager();
            PackageInfo info = manager.getPackageInfo(activity.getPackageName(), 0);
            versionName = info.versionName;
        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }

        return versionName;
    }

    public static String getOsVersionString() {
        return Build.VERSION.RELEASE;
    }

    public static int getDeviceWidth() {
        AppActivity activity = Natives.sharedDelegate().mainActivity;

        Display display = activity.getWindowManager().getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);

        return metrics.widthPixels;
    }

    public static int getDeviceHeight() {
        AppActivity activity = Natives.sharedDelegate().mainActivity;

        Display display = activity.getWindowManager().getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);

        return metrics.heightPixels;
    }

    // 외장디스크 남은용량 (MB)
    public static int getFreeDisk()
    {
        // 외장 메모리가 없으면
        if ( !Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
        {
            return -1;
        }
        else
        {
            File path = Environment.getExternalStorageDirectory();
            StatFs stat = new StatFs(path.getPath());
            long blockSize = stat.getBlockSize();
            long availableBlocks = stat.getAvailableBlocks();
            long freeDisk = (blockSize * availableBlocks) / 1024 / 1024;
            return (int)freeDisk;
        }
    }

    /** 와이파이 인지 체크 한다 **/
    public static boolean isWiFi()
    {
        ConnectivityManager manager = (ConnectivityManager)Natives.sharedDelegate().mainActivity.getSystemService(Context.CONNECTIVITY_SERVICE);

        boolean isWifi = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isConnectedOrConnecting();

        if(isWifi)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public static void goURL(final String url)
    {
        Natives.sharedDelegate().handler.post(new Runnable() {
            @Override
            public void run()
            {
                // 인터넷 Url 호출하기
                Natives.sharedDelegate().mainActivity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
            }
        });
    }

    public static void writeMail(final String url)
    {
        Natives.sharedDelegate().handler.post(new Runnable() {
            @Override
            public void run()
            {
                Log.d("Cocos2dxActivity", "nativeCallWriteMail " + url);

                String email = url;
                String subject = "";		/* Your subject here */
                String body = ""; 			/* Your body here */
                String chooserTitle = "";	/* Your chooser title here */

                Intent emailIntent = new Intent(Intent.ACTION_SENDTO, Uri.parse("mailto:" + email));
                emailIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
                emailIntent.putExtra(Intent.EXTRA_TEXT, body);

                Natives.sharedDelegate().mainActivity.startActivity(Intent.createChooser(emailIntent, chooserTitle));

            }
        });
    }

    public static void intentURIstartActivity(final String url)
    {
        Natives.sharedDelegate().handler.post(new Runnable() {

            @Override
            public void run()
            {
                if (!url.startsWith("http://") && !url.startsWith("https://") && !url.startsWith("javascript:"))
                {
                    Intent intent = null;

                    try
                    {
                        intent = Intent.parseUri(url, Intent.URI_INTENT_SCHEME); //IntentURI처리
                        Uri uri = Uri.parse(intent.getDataString());
                        Log.d("Cocos2dxActivity", "intentURIstartActivity IntentURI" + uri);
                        Natives.sharedDelegate().mainActivity.startActivity(new Intent(Intent.ACTION_VIEW, uri)); //해당되는 Activity 실행

                    }
                    catch (URISyntaxException ex)
                    {

                    }
                    catch (ActivityNotFoundException e)
                    {
                        if ( intent == null )
                            return;

                        if (handleNotFoundPaymentScheme(intent.getScheme()) )
                            return; //설치되지 않은 앱에 대해 사전 처리(Google Play이동 등 필요한 처리)

                        String packageName = intent.getPackage();

                        if (packageName != null)
                        {
                            Log.d("Cocos2dxActivity", "intentURIstartActivity market" + packageName);
                            //packageName이 있는 경우에는 Google Play에서 검색을 기본
                            Natives.sharedDelegate().mainActivity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + packageName)));
                            return;
                        }
                    }
                }
            }
        });
    }

    public static boolean nativeCalSstartsWith(String strUrl, String strKey)
    {
        return strUrl.startsWith(strKey);
    }

    public static boolean isEmailPatternMatching(String str)
    {
        String pattern = "^[_a-zA-Z0-9-\\.]+@[\\.a-zA-Z0-9-]+\\.[a-zA-Z]+$";
        return Pattern.matches(pattern, str);
    }

    public static boolean isPhoneNumberPatternMatching(String str)
    {
        String pattern = "^[0-9-]{10,14}$";
        return Pattern.matches(pattern, str);
    }

    public static boolean  isPasswordPatternMatching(String str)
    {
        String pattern = "^[A-Za-z0-9]{5,15}";
        return Pattern.matches(pattern, str);
    }

    public static boolean  checkValidateString(String str)
    {
        String pattern = "^[\\.,<>?/'\"~*&(){}|_`:;!@#$%^*+=₩\\-\\[\\]\\\\ㆍ]*$";

        if(Pattern.matches(pattern, str))
        {
            return true;
        }

        return false;
    }

    public static boolean isVirtualButton()
    {
        return Natives.sharedDelegate().mainActivity.isVirtualButton();
    }

	/*
	 * Facebook
	 */

    public static void nativeCallFacebookLogin()
    {
        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
        mainActivity.nativeCallFacebookLogin();
    }

    public static void nativeCallFacebookLogOut()
    {
        LoginManager.getInstance().logOut();
    }

    public static boolean nativeCallFacebookLoginCheck()
    {
        AccessToken accessToken = AccessToken.getCurrentAccessToken();
        if (accessToken == null || accessToken.isExpired()) {
            return false;
        } else {
            return true;
        }
    }

    public static void nativeCallFacebookMyInfo()
    {
        AccessToken accessToken = AccessToken.getCurrentAccessToken();

        GraphRequest request = GraphRequest.newMeRequest(accessToken, new GraphJSONObjectCallback() {

            @Override
            public void onCompleted(JSONObject user, GraphResponse response) {

                if (response.getError() != null)
                {
                    Log.d("Cocos2dxActivity", "FacebookMyInfo response Error");
                }
                else
                {
                    Log.d("Cocos2dxActivity", "FacebookMyInfo" + user.toString());
                    Natives.glthread_FacebookMyInfo(user.toString());
                }
            }
        });

        Bundle parameters = new Bundle();
        parameters.putString("fields", "id, email, picture, birthday, gender, link, first_name, last_name, name");
        request.setParameters(parameters);
        request.executeAsync();
    }

    public static void nativeCallFacebookonUserAlbums()
    {
        AccessToken accessToken = AccessToken.getCurrentAccessToken();

        GraphRequest request = new GraphRequest(
                accessToken,
                "me/albums",
                null,
                HttpMethod.GET,
                new GraphRequest.Callback() {

                    @Override
                    public void onCompleted(GraphResponse response) {
			            /* handle the result */

                        JSONObject jsonObject = response.getJSONObject();
                        Log.d("Cocos2dxActivity", "FacebookonUserAlbums" + jsonObject.toString());
                        Natives.glthread_FacebookUserAlbums(jsonObject.toString());
                    }

                });

        Bundle parameters = new Bundle();
        parameters.putString("fields", "count, name");
        request.setParameters(parameters);
        request.executeAsync();
    }

    public static void nativeCallFacebookonUserImages(String strAlbumID)
    {
        final String albumID = strAlbumID;

        String path = "/" + strAlbumID + "/photos";

        AccessToken accessToken = AccessToken.getCurrentAccessToken();

        GraphRequest request = new GraphRequest(
                accessToken,
                path,
                null,
                HttpMethod.GET,
                new GraphRequest.Callback() {

                    @Override
                    public void onCompleted(GraphResponse response) {
			            /* handle the result */

                        JSONObject jsonObject = response.getJSONObject();
                        Log.d("Cocos2dxActivity", "FacebookonUserImages" + jsonObject.toString());
                        Natives.glthread_FacebookUserAlbums(albumID, jsonObject.toString());
                    }

                });

        Bundle parameters = new Bundle();
        parameters.putString("fields", "name, source, id, updated_time, width, height, picture");
        request.setParameters(parameters);
        request.executeAsync();
    }


    public static void nativeCallFacebookSharePhotoData(byte[] buffers, int nSize) {

        final Bitmap image = BitmapFactory.decodeByteArray( buffers, 0, nSize );

        new Thread(new Runnable() {
            @Override
            public void run() {
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {

                        SharePhoto photo = new SharePhoto.Builder()
                                .setBitmap(image)
                                .build();

                        SharePhotoContent content = new SharePhotoContent.Builder()
                                .addPhoto(photo)
                                .build();

                        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
                        ShareDialog shareDialog = new ShareDialog(mainActivity);

                        shareDialog.show(content);
                    }
                });
            }
        }).start();
    }

    public static void nativeCallFacebookSharePhotoPath(String strPath) {

        final String path = strPath;

        new Thread(new Runnable() {
            @Override
            public void run() {
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {

                        Bitmap image = BitmapFactory.decodeFile(path);

                        SharePhoto photo = new SharePhoto.Builder()
                                .setBitmap(image)
                                .build();

                        SharePhotoContent content = new SharePhotoContent.Builder()
                                .addPhoto(photo)
                                .build();

                        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
                        ShareDialog shareDialog = new ShareDialog(mainActivity);

                        shareDialog.show(content);
                    }
                });
            }
        }).start();
    }

    public static void nativeCallFacebookShare(String strUrl) {

        final String url = strUrl;

        new Thread(new Runnable() {
            @Override
            public void run() {
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {

                        AppActivity mainActivity = Natives.sharedDelegate().mainActivity;
                        ShareDialog shareDialog = new ShareDialog(mainActivity);
                        ShareLinkContent linkContent = new ShareLinkContent.Builder()
                                .setContentUrl(Uri.parse(url))
                                .build();

                        shareDialog.show(linkContent);
                    }
                });
            }
        }).start();
    }

    public static String getManuFacturer() {
        return Build.MANUFACTURER;
    }

    public static String getModelName() {

        return Build.MODEL;
    }

    public static String getDeviceBrand() {
        return Build.BRAND;
    }

    public static String getPhoneNumber() {
        TelephonyManager mTelephonyMgr = (TelephonyManager) Natives.sharedDelegate().mainActivity.getBaseContext()
                .getSystemService(Context.TELEPHONY_SERVICE);
        if (mTelephonyMgr.getLine1Number() != null) // 비행기모드일 경우 null 들어올 수 있음.
        {
            return mTelephonyMgr.getLine1Number();
        }

        return "";
    }

    // 사진첩 열기
    public static void showImagePicker(final String savePhotoName)
    {
        Natives.sharedDelegate().handler.post(new Runnable() {
            @Override
            public void run()
            {
                Natives.sharedDelegate().mainActivity.imagePickerView.imageName = savePhotoName;
                Natives.sharedDelegate().mainActivity.imagePickerView.selectMode();
            }
        });
    }

    /**
     * @param scheme
     * @return 해당 scheme에 대해 처리를 직접 하는지 여부
     *
     * 결제를 위한 3rd-party 앱이 아직 설치되어있지 않아 ActivityNotFoundException이 발생하는 경우 처리합니다.
     * 여기서 handler되지않은 scheme에 대해서는 intent로부터 Package정보 추출이 가능하다면 다음에서 packageName으로 market이동합니다.
     *
     */
    public static boolean handleNotFoundPaymentScheme(final String scheme)
    {
        //PG사에서 호출하는 url에 package정보가 없어 ActivityNotFoundException이 난 후 market 실행이 안되는 경우
        if ( PaymentScheme.ISP.equalsIgnoreCase(scheme) )
        {
            Log.d("Cocos2dxActivity", "handleNotFoundPaymentScheme 1" + PaymentScheme.PACKAGE_ISP);
            Natives.sharedDelegate().mainActivity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + PaymentScheme.PACKAGE_ISP)));
            return true;
        } else if ( PaymentScheme.BANKPAY.equalsIgnoreCase(scheme) ) {
            Log.d("Cocos2dxActivity", "handleNotFoundPaymentScheme 1" + PaymentScheme.PACKAGE_BANKPAY);
            Natives.sharedDelegate().mainActivity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + PaymentScheme.PACKAGE_BANKPAY)));
            return true;
        }

        return false;
    }

    public static void nativeCallGoogleSignIn()
    {
        Natives.sharedDelegate().mainActivity.googleSignIn();
    }

    public static void nativeCallGoogleSignOut()
    {
        Natives.sharedDelegate().mainActivity.googleSignOut();
    }

    public static void nativeCallOneSignalsetSubscription(boolean enable)
    {
        OneSignal.setSubscription(enable);
    }

    public static void nativeCallIntroFinish(boolean var)
    {
        Natives.sharedDelegate().mbln_IntroFinish = var;
    }
}
