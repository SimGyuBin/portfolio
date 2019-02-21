/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.request.animation.GlideAnimation;
import com.bumptech.glide.request.target.SimpleTarget;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.kakao.network.ErrorResult;
import com.kakao.usermgmt.UserManagement;
import com.kakao.usermgmt.callback.UnLinkResponseCallback;
import com.onesignal.OneSignal;
import com.sdkbox.plugin.SDKBox;

import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.Point;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.PowerManager;
import android.provider.MediaStore;
import android.provider.MediaStore.MediaColumns;
import android.text.TextUtils;
import android.util.Log;

class WakeUpScreen {

    private static PowerManager.WakeLock wakeLock;

    /**
     * timeout을 설정하면, 자동으로 릴리즈됨
     * @param context
     * @param timeout
     */
    public static void acquire(Context context, long timeout) {

        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        wakeLock = pm.newWakeLock(
                PowerManager.ACQUIRE_CAUSES_WAKEUP  |
                        PowerManager.FULL_WAKE_LOCK         |
                        PowerManager.ON_AFTER_RELEASE
                , context.getClass().getName());

        if(timeout > 0)
            wakeLock.acquire(timeout);
        else
            wakeLock.acquire();

    }

    /**
     * 이 메소드를 사용하면, 반드시 release를 해줘야 함
     * @param context
     */
    public static void acquire(Context context) {
        acquire(context, 0);
    }

    public static void release() {
        if (wakeLock.isHeld())
            wakeLock.release();
    }
}

public class AppActivity extends Cocos2dxActivity {

    public Cocos2dxGLSurfaceView mGLView = null;

    public CallbackManager callbackManager = null;
    public ImagePickerView imagePickerView;

    public static final int GOOGLE_SIGN_IN = 7001;
    public static final int GOOGLE_SIGN_OUT = 7002;

    public static final int KAKAO_SIGN_IN = 8001;
    public static final int KAKAo_SIGN_OUT = 8002;

    Handler mHandler = new Handler(Looper.getMainLooper());

    private static final int THUMBNAIL_SIZE = 180;
    private static final int FULLSCREEN_SIZE = 640;
    private static final int kMaxTextureSize = 2048;

    private static final String IAMPORT_SCHEME = "pzle_purchase";

    FilenameFilter mImageNameFilter = new FilenameFilter() {
        @Override
        public boolean accept(File dir, String name) {

            if (name.matches("^.*\\.(?i)(jpg|png|bmp)$")) {
                return true;
            }

            return false;
        }
    };

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        onLoadNativeLibraries();
        SDKBox.init(this);

        FacebookSdk.sdkInitialize(getApplicationContext());
        Natives.setActivity(this);
        mGLView = getGLSurfaceView();

        imagePickerView = new ImagePickerView();

        Intent intent = getIntent();
        Uri intentData = intent.getData();

        if (intentData != null)
        {
            //isp 인증 후 복귀했을 때 결제 후속조치
            String url = intentData.toString();

            if ( url.startsWith(IAMPORT_SCHEME) )
            {
                // "iamportapp://https://pgcompany.com/foo/bar"와 같은 형태로 들어옴
                String redirectURL = url.substring(IAMPORT_SCHEME.length() + "://".length());
                Natives.glthread_purchaseLayerloadURL(redirectURL);
            }
        }

        // 푸시 정보 get
        OneSignal.idsAvailable(new OneSignal.IdsAvailableHandler() {
            @Override
            public void idsAvailable(String userId, String registrationId) {

                String strUserId = "";
                String strRegistrationId = "";

                if (userId != null)
                {
                    Log.d("OneSignal", "PlayerId:" + userId);
                    strUserId = userId;
                }

                if (registrationId != null)
                {
                    Log.d("OneSignal", "registrationId:" + registrationId);
                    strRegistrationId = registrationId;
                }

                Natives.glthread_idsAvailable(strUserId, strRegistrationId);
            }
        });

        intent = getIntent();

        if(intent != null)
        {
            String type = intent.getStringExtra("type");

            if(type != null && type.equals("push"))
            {
                String strMessage = intent.getStringExtra("message");
                String strTitle = intent.getStringExtra("title");
                String push_event = intent.getStringExtra("push_event");

                Natives.glthread_OneSignalHandleNotification(strMessage,strTitle,push_event, false);
            }
        }

        // 뱃지 카운트 삭제
        Intent test = new Intent("android.intent.action.BADGE_COUNT_UPDATE");
        test.putExtra("badge_count_package_name", "com.pixholic.pzle");
        test.putExtra("badge_count_class_name", "AppActivity");
        test.putExtra("badge_count", 0);
        sendBroadcast(intent);

//        try
//        {
//            PackageInfo info = getPackageManager().getPackageInfo(
//                    "com.pixholic.pzle", //앱의 패키지 명
//                    PackageManager.GET_SIGNATURES);
//
//            for (Signature signature : info.signatures)
//            {
//                MessageDigest md = MessageDigest.getInstance("SHA");
//                md.update(signature.toByteArray());
//
//                Log.d("Cocos2dxActivity", "KeyHash:" + Base64.encodeToString(md.digest(),
//                        Base64.DEFAULT));
//            }
//        }
//        catch (PackageManager.NameNotFoundException e)
//        {
//
//        }
//        catch (NoSuchAlgorithmException e)
//        {
//
//        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {

        switch (requestCode)
        {
            case Natives.READ_EXTERNAL_STORAGE_REQUEST:

                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    // 권한 허가
                    // 해당 권한을 사용해서 작업을 진행할 수 있습니다

                    Natives.nativeCallPathOfAllImages();

                }
                else
                {
                    // 권한 거부
                    // 사용자가 해당권한을 거부했을때 해주어야 할 동작을 수행합니다
                }
                return;
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        Log.d("Cocos2dxActivity", "onActivityResult : " + resultCode);

        if (!SDKBox.onActivityResult(requestCode, resultCode, data)) {
            super.onActivityResult(requestCode, resultCode, data);
        }
        else if (callbackManager != null) {
            super.onActivityResult(requestCode, resultCode, data);
            callbackManager.onActivityResult(requestCode, resultCode, data);
        }
        else
        {
            super.onActivityResult(requestCode, resultCode, data);

            if (resultCode == RESULT_OK )
            {
                //imagePickerView.onResult(requestCode, data);
            }
            else if(resultCode == GOOGLE_SIGN_IN)
            {
                if(data != null)
                {
                    boolean isSuccess = data.getBooleanExtra("isSuccess", false);

                    if(isSuccess)
                    {
                        String displayName = data.getStringExtra("getDisplayName");

                        if(displayName == null)
                        {
                            displayName = "";
                        }
                        String Id = data.getStringExtra("getId");

                        if(Id == null)
                        {
                            Id = "";
                        }

                        String Email =  data.getStringExtra("getEmail");

                        if(Email == null)
                        {
                            Email = "";
                        }

                        String token = data.getStringExtra("getIdToken");

                        if(token == null)
                        {
                            token = "";
                        }

                        Natives.glthread_GoogleSignInSuccess(displayName, Id, Email, token);
                    }
                    else
                    {

                    }
                }
            }
            else if(resultCode == GOOGLE_SIGN_OUT)
            {

            }
            else if(resultCode == KAKAO_SIGN_IN)
            {
                if(data != null) {
                    boolean isSuccess = data.getBooleanExtra("isSuccess", false);

                    if (isSuccess)
                    {

                        String Id = data.getStringExtra("getId");

                        if(Id == null)
                        {
                            Id = "";
                        }

                        String Nickname = data.getStringExtra("getNickname");

                        if(Nickname == null)
                        {
                            Nickname = "";
                        }

                        String Email =  data.getStringExtra("getEmail");

                        if(Email == null)
                        {
                            Email = "";
                        }

                        Natives.glthread_KakaoLoginSuccess(Id, Nickname, Email);
                    }
                    else
                    {

                    }
                }


            }
            else
            {
                // c++로 실패 여부 알려주기
                //Natives.glthread_CancelPicture();
            }
        }
    }

    @Override
    protected void onNewIntent(Intent intent) {
        String url = intent.toString();

        Log.d("Cocos2dxActivity", "onNewIntent : " + url);

        if ( url.startsWith(IAMPORT_SCHEME) )
        {
            // "iamportapp://https://pgcompany.com/foo/bar"와 같은 형태로 들어옴
            String redirectURL = url.substring(IAMPORT_SCHEME.length() + "://".length());
            Natives.glthread_purchaseLayerloadURL(redirectURL);
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        SDKBox.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
        SDKBox.onStop();
    }

    @Override
    protected void onResume() {
        super.onResume();
        SDKBox.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        SDKBox.onPause();
    }

    @Override
    public void onBackPressed() {
        if (!SDKBox.onBackPressed()) {
            super.onBackPressed();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        //WakeUpScreen.release();
        //android.os.Process.killProcess(android.os.Process.myPid());

    }

    public void googleSignIn()
    {
        Intent googleSignInActiivity = new Intent(AppActivity.this, GoogleSignInActiivity.class);
        googleSignInActiivity.putExtra("type", "signIn");
        googleSignInActiivity.putExtra("resultCode",GOOGLE_SIGN_IN);
        startActivityForResult(googleSignInActiivity,0);
    }

    public void googleSignOut()
    {
        Intent googleSignInActiivity = new Intent(AppActivity.this, GoogleSignInActiivity.class);
        googleSignInActiivity.putExtra("type", "signOut");
        googleSignInActiivity.putExtra("resultCode",GOOGLE_SIGN_OUT);
        startActivityForResult(googleSignInActiivity,0);
    }

    public void changeOrientation(int flag) {
        if (flag == 0) {
            // 세로
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        } else {
            // 가로
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        }
    }

    public ArrayList<ImageModel> getPathOfAllImages() {

        final String[] columns = { MediaColumns.DATA, MediaColumns.DISPLAY_NAME, MediaStore.Images.Media.DATA, MediaStore.Images.ImageColumns.BUCKET_DISPLAY_NAME, MediaColumns.DATE_MODIFIED,
                MediaColumns.DATE_ADDED, MediaColumns.SIZE };
        final String orderBy = MediaStore.Images.Media.DATE_ADDED + " desc";
        Cursor imageCursor = getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, columns, null,
                null, orderBy);

        int columnIndex = imageCursor.getColumnIndexOrThrow(MediaColumns.DATA);
        int columnDisplayname = imageCursor.getColumnIndexOrThrow(MediaColumns.DISPLAY_NAME);
        int columnDateModified = imageCursor.getColumnIndexOrThrow(MediaColumns.DATE_MODIFIED);
        int columnDateAdded = imageCursor.getColumnIndexOrThrow(MediaColumns.DATE_ADDED);
        int columnSize = imageCursor.getColumnIndexOrThrow(MediaColumns.SIZE);


        ArrayList<ImageModel> result = new ArrayList<ImageModel>();

        if (imageCursor != null && imageCursor.getCount() > 0) {

            int nCount = imageCursor.getCount();
            imageCursor.moveToFirst();

            int lastIndex;

            for (int i = 0; i < nCount; i++) {

                imageCursor.getBlob(columnIndex);

                String absolutePathOfImage = imageCursor.getString(columnIndex);
                String nameOfFile = imageCursor.getString(columnDisplayname);
                String category_name = imageCursor.getString(imageCursor.getColumnIndex(MediaStore.Images.ImageColumns.BUCKET_DISPLAY_NAME));
                String data = imageCursor.getString(imageCursor.getColumnIndex(MediaStore.Images.ImageColumns.DATA));
                String path = data.replaceFirst("/.[^/]+$", "");
                String category_path = "";

                File file = new File(path);

                if (file != null)
                {
                    category_path = file.getPath();
                }

                // String dateModified =
                // imageCursor.getString(columnDateModified);
                // String dateAdded = imageCursor.getString(columnDateAdded);
                Date dateModified = new Date(imageCursor.getLong(columnDateModified) * 1000);
                Date dateAdded = new Date(imageCursor.getLong(columnDateAdded) * 1000);

                String size = imageCursor.getString(columnSize);

                lastIndex = absolutePathOfImage.lastIndexOf(nameOfFile);
                lastIndex = lastIndex >= 0 ? lastIndex : nameOfFile.length() - 1;

                if (!TextUtils.isEmpty(absolutePathOfImage) && !getExtension(absolutePathOfImage).equals("gif")) {

                    ImageModel model = new ImageModel();

                    // model.path = String.valueOf(i) + "_" + size + "_" +
                    // absolutePathOfImage;
                    model.path = absolutePathOfImage;
                    model.date_modified = dateModified.toString();
                    model.date_added = dateAdded.toString();

                    model.size = size;

                    int imageHeight = 0;
                    int imageWidth = 0;
                    String dateString = null;

                    try {
                        BitmapFactory.Options options = new BitmapFactory.Options();
                        options.inJustDecodeBounds = true;
                        // options.inSampleSize = 2;
                        BitmapFactory.decodeFile(new File(absolutePathOfImage).getAbsolutePath(), options);

                        imageHeight = options.outHeight;
                        imageWidth = options.outWidth;

                    } catch (Exception e) {

                        Log.d("Cocos2dxActivity", "BitmapFactory Options Exception");
                    }

                    model.width = String.valueOf(imageWidth);
                    model.height = String.valueOf(imageHeight);

                    try {

                        ExifInterface exif = new ExifInterface(absolutePathOfImage);

                        int orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, 1);

                        if (orientation == ExifInterface.ORIENTATION_ROTATE_90
                                || orientation == ExifInterface.ORIENTATION_ROTATE_270) {
                            String strTemp = model.width;
                            model.width = model.height;
                            model.height = strTemp;
                        }

                        dateString = exif.getAttribute(ExifInterface.TAG_DATETIME);

                    } catch (Exception e) {

                    }

                    if (dateString == null || dateString.length() == 0)
                    {
                        SimpleDateFormat transFormat = new SimpleDateFormat("yyyyMMddHHmmss");
                        dateString = transFormat.format(dateAdded);
                    }
                    else
                    {
                        String strDateTime = dateString;
                        dateString = "";
                        strDateTime = strDateTime.trim();
                        strDateTime = strDateTime.replaceAll(" ", "");
                        String[] array;
                        array = strDateTime.split(":");

                        for(int j = 0; j < array.length ; ++j)
                        {
                            dateString += array[j];
                        }
                    }

                    model.fulldate = dateString;
                    model.category_name = category_name;
                    model.category_path = category_path;
                    result.add(model);
                }

                imageCursor.moveToNext();
            }
        }

        return result;
    }


    public static String getExtension(String fileStr) {

        return fileStr.substring(fileStr.lastIndexOf(".")+1,fileStr.length());

    }

    public void createThumbnailImage(String strUrl, int width, int height) {
        final String absolutePathOfImage = strUrl;

        if (!TextUtils.isEmpty(absolutePathOfImage)) {

            final String uri = "file://" + absolutePathOfImage;

//			AsyncTask.execute( new Runnable(){
//				public void run() {
//
//					new Handler(Looper.getMainLooper()).post(new Runnable() {
//						@Override
//						public void run() {
//
//							createThumbnailBitmap(uri, absolutePathOfImage);
//						}
//					});
//				}
//			});


//			new AsyncTask<Void, Void, Void>() {
//				@Override
//				protected Void doInBackground(Void... params) {
//
//					BitmapFactory.Options options = new BitmapFactory.Options();
//					options.inJustDecodeBounds = false;
//					options.inSampleSize = 4;
//					options.inPreferredConfig = Bitmap.Config.RGB_565;
//
//					Bitmap bmp = BitmapFactory.decodeFile(absolutePathOfImage, options);
//
//					Bitmap thumbnailBitmap = null;
//					int reqHeight = THUMBNAIL_SIZE;
//					 int reqWidth = THUMBNAIL_SIZE;
//					 int nWidth = bmp.getWidth();
//					 int nHeight = bmp.getHeight();
//
//					 float fScale = 1.0f;
//
//					 if (nWidth > reqHeight || nHeight > reqWidth) {
//
//						 float tw = ((float) THUMBNAIL_SIZE / ((float) nWidth));
//						 float th = ((float) THUMBNAIL_SIZE / ((float) nHeight));
//
//						 if (tw > th)
//						 {
//							 fScale = tw;
//						 }
//						 else
//						 {
//							 fScale = th;
//						 }
//					 }
//
//					 Matrix matrix = getExifRotation(absolutePathOfImage);
//
//					 if (matrix != null) {
//
//						 matrix.postScale(fScale, fScale);
//						 thumbnailBitmap = Bitmap.createBitmap(bmp, 0, 0, nWidth, nHeight,
//						 matrix, true);
//
//					 } else {
//
//						 matrix = new Matrix();
//						 matrix.postScale(fScale, fScale);
//						 thumbnailBitmap = Bitmap.createBitmap(bmp, 0, 0, nWidth, nHeight,
//						 matrix, false);
//
//					 }
//
//					 String thumbnailWidth = String.valueOf(thumbnailBitmap.getWidth());
//					 String thumbnailHeight = String.valueOf(thumbnailBitmap.getHeight());
//
//					 byte[] byteArray = bitmapToByteArray(thumbnailBitmap);
//					 Natives.glthread_GetThumbnailImage(byteArray, (int)byteArray.length, absolutePathOfImage, thumbnailWidth, thumbnailHeight);
//
//					 thumbnailBitmap.recycle();
//					 bmp.recycle();
//
//					return null;
//				}
//			}.execute();

            mHandler.post(new Runnable() {
                @Override
                public void run() {

                    createThumbnailBitmap(uri, absolutePathOfImage);
                }
            });

            // int reqHeight = THUMBNAIL_SIZE;
            // int reqWidth = THUMBNAIL_SIZE;
            //
            // int nWidth = width;
            // int nHeight = height;
            //
            // Matrix matrix = null;
            //
            // if (nHeight > reqHeight || nWidth > reqWidth)
            // {
            // float tw = ((float) THUMBNAIL_SIZE / ((float) nWidth));
            // float th = ((float) THUMBNAIL_SIZE / ((float) nHeight));
            //
            // float fScale = 1.0f;
            //
            // if (tw > th)
            // {
            // fScale = tw;
            // }
            // else
            // {
            // fScale = th;
            // }
            //
            // nWidth = (int)(nWidth * fScale);
            // nHeight = (int)(nHeight * fScale);
            // }

            // Glide.with(getContext())
            // .load(url)
            // .asBitmap()
            // .into(new SimpleTarget<Bitmap>(nWidth, nHeight) {
            //
            // @Override
            // public void onResourceReady(Bitmap arg0, GlideAnimation<? super
            // Bitmap> arg1) {
            // // TODO Auto-generated method stub
            //
            // }
            // });
        }
    }

    public void createThumbnailBitmap(final String uri, final String absolutePathOfImage)
    {

//		Glide.with(this)
//	    .load(uri)
//	    .asBitmap()
//	    .toBytes()
//	    .centerCrop()
//	    .into(new SimpleTarget<byte[]>(THUMBNAIL_SIZE, THUMBNAIL_SIZE) {
//	        @Override
//	        public void onResourceReady(byte[] data, GlideAnimation anim) {
//	            // Post your bytes to a background thread and upload them here.
//
//	        	int imageHeight = 0;
//				int imageWidth = 0;
//
//				try {
//					BitmapFactory.Options options = new BitmapFactory.Options();
//					options.inJustDecodeBounds = true;
//					// options.inSampleSize = 2;
//					BitmapFactory.decodeFile(new File(absolutePathOfImage).getAbsolutePath(), options);
//
//					imageHeight = options.outHeight;
//					imageWidth = options.outWidth;
//
//				} catch (Exception e) {
//
//					Log.d("Cocos2dxActivity", "BitmapFactory Options Exception");
//				}
//
//				String thumbnailWidth = String.valueOf(imageWidth);
//				String thumbnailHeight = String.valueOf(imageHeight);
//
//	        	Natives.glthread_GetThumbnailImage(data, (int)data.length, absolutePathOfImage, thumbnailWidth, thumbnailHeight);
//	        }
//	    });


//		long imageId = -1;
//
//		try
//		{
//		    String[] projection = new String[] { MediaStore.MediaColumns._ID };
//		    String selection = MediaStore.MediaColumns.DATA + "=?";
//		    String[] selectionArgs = new String[] { absolutePathOfImage };
//		    Cursor cursor = getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, projection, selection, selectionArgs, null);
//
//		    if (cursor != null && cursor.moveToFirst())
//		    {
//		    	imageId = cursor.getInt(cursor.getColumnIndex(MediaStore.MediaColumns._ID));
//		    }
//		    cursor.close();
//
//
//		}
//		catch (Exception e)
//		{
//			e.printStackTrace();
//		}
//
//
//		Integer uid = ((Number)imageId).intValue();
//		new LoadImageAsyncTask(this, uid ,absolutePathOfImage).execute();





        try {

            ExifInterface exif = new ExifInterface(absolutePathOfImage);

            byte[] byteArray = exif.getThumbnail();
            int orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_UNDEFINED);

            if(orientation != ExifInterface.ORIENTATION_NORMAL)
            {
                Bitmap rotateBitmap = rotateBitmap(byteArrayToBitmap(byteArray), orientation);
                byteArray = bitmapToByteArray(rotateBitmap);
            }

            Natives.glthread_GetThumbnailImage(byteArray, (int)byteArray.length, absolutePathOfImage, "0", "0");

            return;

        } catch (Exception e) {


        }



//		String thumnailPath = getThumnailPath(this, uri);
//
//		Log.d("Cocos2dxActivity", "getThumnailPath uri : " + uri);
//		Log.d("Cocos2dxActivity", "getThumnailPath thum : " + thumnailPath);
//		Log.d("Cocos2dxActivity", "~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
//
        Glide.with(this)
                .load(uri)
                .asBitmap()
                .into(new SimpleTarget<Bitmap>(THUMBNAIL_SIZE,THUMBNAIL_SIZE) {
                    @Override

                    public void onResourceReady(Bitmap resource, GlideAnimation<? super Bitmap> glideAnimation) {

                        final Bitmap thumbnailBitmap = resource;

                        new AsyncTask<Object, Void, byte[]>() {

                            @Override
                            protected byte[] doInBackground(Object... params) {

//    	            	String thumbnailWidth = String.valueOf(thumbnailBitmap.getWidth());
//    	            	String thumbnailHeight = String.valueOf(thumbnailBitmap.getHeight());

                                return bitmapToByteArray(thumbnailBitmap);
                            }

                            @Override
                            protected void onPostExecute(byte[] result) {

//    					thumbnailBitmap.recycle();
                                Natives.glthread_GetThumbnailImage(result, (int)result.length, absolutePathOfImage, "0", "0");
                            }

                        }.execute();




                        // you can do something with loaded bitmap here

//            	Bitmap thumbnailBitmap = resource;
//
//            	String thumbnailWidth = String.valueOf(thumbnailBitmap.getWidth());
//            	String thumbnailHeight = String.valueOf(thumbnailBitmap.getHeight());
//
//            	byte[] byteArray = bitmapToByteArray(thumbnailBitmap);
//            	Natives.glthread_GetThumbnailImage(byteArray, (int)byteArray.length, absolutePathOfImage, thumbnailWidth, thumbnailHeight);
//
//				thumbnailBitmap.recycle();
                    }
                });
    }

    public void createFullScreenImage(String strUrl, int width, int height) {
        final String absolutePathOfImage = strUrl;

        if (!TextUtils.isEmpty(absolutePathOfImage)) {

            final String uri = "file://" + absolutePathOfImage;

//			new Thread(new Runnable() {
//				@Override
//				public void run() {
//					new Handler(Looper.getMainLooper()).post(new Runnable() {
//						@Override
//						public void run() {
//
//							createFullScreenBitmap(uri, absolutePathOfImage);
//						}
//					});
//				}
//			}).start();

            mHandler.post(new Runnable() {
                @Override
                public void run() {

                    createFullScreenBitmap(uri, absolutePathOfImage);
                }
            });
        }
    }

    public void createFullScreenBitmap(final String uri, final String absolutePathOfImage)
    {

//		Glide.with(this)
//	    .load(uri)
//	    .asBitmap()
//	    .toBytes()
//	    .centerCrop()
//	    .into(new SimpleTarget<byte[]>(FULLSCREEN_SIZE,FULLSCREEN_SIZE) {
//	        @Override
//	        public void onResourceReady(byte[] data, GlideAnimation anim) {
//	            // Post your bytes to a background thread and upload them here.
//
//	        	int imageHeight = 0;
//				int imageWidth = 0;
//
//				try {
//					BitmapFactory.Options options = new BitmapFactory.Options();
//					options.inJustDecodeBounds = true;
//					// options.inSampleSize = 2;
//					BitmapFactory.decodeFile(new File(absolutePathOfImage).getAbsolutePath(), options);
//
//					imageHeight = options.outHeight;
//					imageWidth = options.outWidth;
//
//				} catch (Exception e) {
//
//					Log.d("Cocos2dxActivity", "BitmapFactory Options Exception");
//				}
//
//				String originalWidth = String.valueOf(imageWidth);
//				String originalHeight = String.valueOf(imageHeight);
//
//	        	Natives.glthread_GetFullScreenImage(data, (int)data.length, absolutePathOfImage, originalWidth, originalHeight);
//	        }
//	    });

        Glide.with(this)
                .load(uri)
                .asBitmap()
                .into(new SimpleTarget<Bitmap>(FULLSCREEN_SIZE,FULLSCREEN_SIZE) {

                    @Override
                    public void onResourceReady(Bitmap resource, GlideAnimation<? super Bitmap> glideAnimation) {

                        final Bitmap originalBitmap = resource;

                        new AsyncTask<Void, Void, Void>() {

                            @Override
                            protected Void doInBackground(Void... params) {

                                String originalWidth = String.valueOf(originalBitmap.getWidth());
                                String originalHeight = String.valueOf(originalBitmap.getHeight());

                                byte[] byteArray = bitmapToByteArray(originalBitmap);
                                Natives.glthread_GetFullScreenImage(byteArray, (int) byteArray.length, absolutePathOfImage, originalWidth, originalHeight);

//    	            	originalBitmap.recycle();

                                return null;
                            }
                        }.execute();
                    }
                });
    }

    public void createUploadImage(String strUrl) {
        final String absolutePathOfImage = strUrl;

        if (!TextUtils.isEmpty(absolutePathOfImage)) {

            new LoadImageAsyncTask(this, absolutePathOfImage).execute();

//			final String uri = "file://" + absolutePathOfImage;

//			new Thread(new Runnable() {
//				@Override
//				public void run() {
//					new Handler(Looper.getMainLooper()).post(new Runnable() {
//						@Override
//						public void run() {
//
//							createUploadBitmap(uri, absolutePathOfImage, datetime);
//						}
//					});
//				}
//			}).start();
        }
    }

    public void createUploadBitmap(final String uri, final String absolutePathOfImage, final String datetime)
    {
        Glide.with(this)
                .load(uri)
                .asBitmap()
                .toBytes()
                .centerCrop()
                .into(new SimpleTarget<byte[]>() {
                    @Override
                    public void onResourceReady(byte[] data, GlideAnimation anim) {
                        // Post your bytes to a background thread and upload them here.

                        int imageHeight = 0;
                        int imageWidth = 0;

                        try {
                            BitmapFactory.Options options = new BitmapFactory.Options();
                            options.inJustDecodeBounds = true;
                            // options.inSampleSize = 2;
                            BitmapFactory.decodeFile(new File(absolutePathOfImage).getAbsolutePath(), options);

                            imageHeight = options.outHeight;
                            imageWidth = options.outWidth;

                        } catch (Exception e) {

                            Log.d("Cocos2dxActivity", "BitmapFactory Options Exception");
                        }

                        String originalWidth = String.valueOf(imageWidth);
                        String originalHeight = String.valueOf(imageHeight);

                        Natives.glthread_GetUploadImage(data, (int) data.length, absolutePathOfImage,
                                originalWidth, originalHeight, datetime);
                    }
                });

//		Glide.with(this)
//        .load(uri)
//        .asBitmap()
//        .into(new SimpleTarget<Bitmap>() {
//            @Override
//
//            public void onResourceReady(Bitmap resource, GlideAnimation<? super Bitmap> glideAnimation) {
//                // you can do something with loaded bitmap here
//
//            	Bitmap originalBitmap = resource;
//
//            	String originalWidth = String.valueOf(originalBitmap.getWidth());
//				String originalHeight = String.valueOf(originalBitmap.getHeight());
//
//				byte[] byteArray = bitmapToByteArray(originalBitmap);
//				Natives.glthread_GetUploadImage(byteArray, (int) byteArray.length, absolutePathOfImage,
//						originalWidth, originalHeight, datetime);
//
//				originalBitmap.recycle();
//            }
//        });
    }

    public Matrix getExifRotation(String path) {
        try {
            ExifInterface exif = new ExifInterface(path);

            int orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, 1);

            Matrix matrix = new Matrix();

            if (orientation == ExifInterface.ORIENTATION_ROTATE_90) {
                matrix.postRotate(90);
                return matrix;
            } else if (orientation == ExifInterface.ORIENTATION_ROTATE_180) {
                matrix.postRotate(180);
                return matrix;
            } else if (orientation == ExifInterface.ORIENTATION_ROTATE_270) {
                matrix.postRotate(270);
                return matrix;
            }

            // return matrix;

            // myBitmap = Bitmap.createBitmap(myBitmap, 0, 0,
            // myBitmap.getWidth(), myBitmap.getHeight(), matrix, true); //
            // rotating bitmap
        } catch (Exception e) {

        }

        return null;
    }

    public Bitmap getThumbnailBitmap(Bitmap bm) {
        Bitmap resizedBitmap = bm;

        int width = bm.getWidth();
        int height = bm.getHeight();

        if (width > kMaxTextureSize || height > kMaxTextureSize) {
            float tw = ((float) THUMBNAIL_SIZE / ((float) width));
            float th = ((float) THUMBNAIL_SIZE / ((float) height));

            float fScale = 1.0f;

            if (tw > th) {
                fScale = tw;
            } else {
                fScale = th;
            }

            // create a matrix for the manipulation
            Matrix matrix = new Matrix();

            // resize the bit map
            matrix.postScale(fScale, fScale);

            // recreate the new Bitmap
            resizedBitmap = Bitmap.createBitmap(bm, 0, 0, width, height, matrix, false);
        }

        return resizedBitmap;
    }

    public static Bitmap rotateBitmap(Bitmap bitmap, int orientation) {

        try{
            Matrix matrix = new Matrix();
            switch (orientation) {
                case ExifInterface.ORIENTATION_NORMAL:
                    return bitmap;
                case ExifInterface.ORIENTATION_FLIP_HORIZONTAL:
                    matrix.setScale(-1, 1);
                    break;
                case ExifInterface.ORIENTATION_ROTATE_180:
                    matrix.setRotate(180);
                    break;
                case ExifInterface.ORIENTATION_FLIP_VERTICAL:
                    matrix.setRotate(180);
                    matrix.postScale(-1, 1);
                    break;
                case ExifInterface.ORIENTATION_TRANSPOSE:
                    matrix.setRotate(90);
                    matrix.postScale(-1, 1);
                    break;
                case ExifInterface.ORIENTATION_ROTATE_90:
                    matrix.setRotate(90);
                    break;
                case ExifInterface.ORIENTATION_TRANSVERSE:
                    matrix.setRotate(-90);
                    matrix.postScale(-1, 1);
                    break;
                case ExifInterface.ORIENTATION_ROTATE_270:
                    matrix.setRotate(-90);
                    break;
                default:
                    return bitmap;
            }
            try {
                Bitmap bmRotated = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
                bitmap.recycle();
                return bmRotated;
            }
            catch (OutOfMemoryError e) {
                e.printStackTrace();
                return null;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static String uriToThumbnail(Context context, String imageId) {
        // DATA는 이미지 파일의 스트림 데이터 경로를 나타냅니다.
        String[] projection = { MediaStore.Images.Thumbnails.DATA };
        ContentResolver contentResolver = context.getContentResolver();

        // 원본 이미지의 _ID가 매개변수 imageId인 썸네일을 출력
        Cursor thumbnailCursor = contentResolver.query(
                MediaStore.Images.Thumbnails.EXTERNAL_CONTENT_URI, // 썸네일 컨텐트 테이블
                projection, // DATA를 출력
                MediaStore.Images.Thumbnails.IMAGE_ID + "=?", // IMAGE_ID는 원본 이미지의 _ID를 나타냅니다.
                new String[]{ imageId },
                null);
        if (thumbnailCursor == null) {
            // Error 발생
            // 적절하게 handling 해주세요
            return null;
        } else if (thumbnailCursor.moveToFirst()) {
            int thumbnailColumnIndex = thumbnailCursor.getColumnIndex(projection[0]);

            String thumbnailPath = thumbnailCursor.getString(thumbnailColumnIndex);
            thumbnailCursor.close();
            return thumbnailPath;
        } else {
            // thumbnailCursor가 비었습니다.
            // 이는 이미지 파일이 있더라도 썸네일이 존재하지 않을 수 있기 때문입니다.
            // 보통 이미지가 생성된 지 얼마 되지 않았을 때 그렇습니다.
            // 썸네일이 존재하지 않을 때에는 아래와 같이 썸네일을 생성하도록 요청합니다
            MediaStore.Images.Thumbnails.getThumbnail(contentResolver, Long.parseLong(imageId), MediaStore.Images.Thumbnails.MINI_KIND, null);
            thumbnailCursor.close();
            return uriToThumbnail(context, imageId);
        }
    }

    /**
     * get thumbnail path
     *
     * @param context
     * @param path
     *          /storage/sdcard0/DCIM/100LGDSC/CAM00039.jpg
     * @return /storage/sdcard0/DCIM/.thumbnails/1425454566357.jpg
     */
    public static String getThumnailPath(Context context, String path)
    {
        String result = null;
        long imageId = -1;

        try
        {
            String[] projection = new String[] { MediaStore.MediaColumns._ID };
            String selection = MediaStore.MediaColumns.DATA + "=?";
            String[] selectionArgs = new String[] { path };
            Cursor cursor = context.getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, projection, selection, selectionArgs, null);
            if (cursor != null && cursor.moveToFirst())
            {
                imageId = cursor.getInt(cursor.getColumnIndex(MediaStore.MediaColumns._ID));
            }
            cursor.close();

            cursor = MediaStore.Images.Thumbnails.queryMiniThumbnail(context.getContentResolver(), imageId, MediaStore.Images.Thumbnails.MINI_KIND, null);
            if (cursor != null && cursor.getCount() > 0)
            {
                cursor.moveToFirst();
                result = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Images.Thumbnails.DATA));
            }
            else
            {
                ContentResolver contentResolver = context.getContentResolver();
                MediaStore.Images.Thumbnails.getThumbnail(contentResolver, imageId, MediaStore.Images.Thumbnails.MINI_KIND, null);
                cursor.close();

                return getThumnailPath(context, path);
            }
            cursor.close();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        if (TextUtils.isEmpty(result))
        {
            result = path;
        }

        return result;
    }


    public static byte[] bitmapToByteArray(Bitmap bitmap) {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        bitmap.compress(CompressFormat.JPEG, 100, stream);
        byte[] byteArray = stream.toByteArray();
        return byteArray;
    }

    public Bitmap byteArrayToBitmap( byte[] byteArray ) {
        Bitmap bitmap = BitmapFactory.decodeByteArray( byteArray, 0, byteArray.length ) ;
        return bitmap ;
    }

    public void nativeCallFacebookLogin() {
        callbackManager = CallbackManager.Factory.create();

        LoginManager.getInstance().logInWithReadPermissions(this, Arrays.asList("public_profile", "email"));
        LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>() {

            @Override
            public void onSuccess(final LoginResult result) {

                Natives.glthread_FacebookLogin(true, "connection result");
                callbackManager = null;
            }

            @Override
            public void onError(FacebookException error) {

                Natives.glthread_FacebookLogin(false, "Process error");
                callbackManager = null;
            }

            @Override
            public void onCancel() {

                Natives.glthread_FacebookLogin(false, "Cancelled");
                callbackManager = null;
            }
        });
    }

    public boolean isVirtualButton()
    {
        Point size = getScreenRatio(this);

        if(size.x >= 18)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public void kakaoLogin()
    {
        Intent kaKaoLoginActivity = new Intent(AppActivity.this, KaKaoLoginActivity.class);
        kaKaoLoginActivity.putExtra("type", "signIn");
        kaKaoLoginActivity.putExtra("resultCode",KAKAO_SIGN_IN);
        startActivityForResult(kaKaoLoginActivity,0);
    }

    public void kakaoLoginOut()
    {
        UserManagement.requestUnlink(new UnLinkResponseCallback() {
            @Override
            public void onFailure(ErrorResult errorResult) {

            }

            @Override
            public void onSessionClosed(ErrorResult errorResult) {

            }

            @Override
            public void onNotSignedUp() {

            }

            @Override
            public void onSuccess(Long userId) {

            }
        });
    }

    public class LoadImageAsyncTask extends AsyncTask<Object, Void, byte[]> {

        private Context mContext;
        private String path;


        public LoadImageAsyncTask(Context context, String path)
        {
            this.mContext = context;
            this.path = path;
        }

        @Override
        protected byte[] doInBackground(Object... params)
        {
            String absolutePath = path;
            Log.d("Cocos2dxActivity","LoadImageAsyncTask file Path=>"+ absolutePath);

            FileInputStream fileInputStream = null;
            FileChannel ch = null;
            byte[] bytes = null;

            try {

                fileInputStream = new FileInputStream(absolutePath);
                ch = fileInputStream.getChannel();
                int size = (int) ch.size();
                bytes = new byte[size];

                ByteArrayOutputStream os = new ByteArrayOutputStream();
                int c;
                while ((c = fileInputStream.read(bytes)) != -1) {
                    os.write(bytes, 0, c);
                }

            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

            return bytes;

        }

        @Override
        protected void onPostExecute(byte[] bytes)
        {
            String absolutePath = path;
            String dateString = null;
            String originalWidth = null;
            String originalHeight = null;

            try {
                BitmapFactory.Options options = new BitmapFactory.Options();
                options.inJustDecodeBounds = true;
                // options.inSampleSize = 2;
                BitmapFactory.decodeFile(new File(absolutePath).getAbsolutePath(), options);

                originalWidth = String.valueOf(options.outHeight);
                originalHeight = String.valueOf(options.outWidth);

            } catch (Exception e) {

                Log.d("Cocos2dxActivity", "BitmapFactory Options Exception");
            }

            try {

                ExifInterface exif = new ExifInterface(absolutePath);

                int orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, 1);

                if (orientation == ExifInterface.ORIENTATION_ROTATE_90
                        || orientation == ExifInterface.ORIENTATION_ROTATE_270) {
                    String strTemp = originalWidth;
                    originalWidth = originalHeight;
                    originalHeight = strTemp;
                }

                dateString = exif.getAttribute(ExifInterface.TAG_DATETIME);

                if (dateString == null || dateString.length() == 0)
                {
                    long date = 0;
                    File file = new File(absolutePath);
                    if (file.exists()) {
                        date = file.lastModified();
                    }

                    SimpleDateFormat transFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    dateString = transFormat.format(date);
                }
                else
                {
                    String[] array;
                    String[] dateArray;
                    String[] timeArray;

                    Log.d("Cocos2dxActivity", "ExifInterface.TAG_DATETIME 1 : " + dateString);

                    String strDateTime = dateString;
                    array = strDateTime.split(" ");

                    if(array.length >= 2)
                    {
                        dateString = "";

                        dateArray = array[0].split(":");
                        timeArray = array[1].split(":");

                        for(int j = 0; j < dateArray.length ; ++j)
                        {
                            dateString += dateArray[j];

                            if((dateArray.length - 1) != j)
                            {
                                dateString += "-";
                            }
                        }

                        dateString += " ";

                        for(int j = 0; j < timeArray.length ; ++j)
                        {
                            dateString += timeArray[j];

                            if((timeArray.length - 1) != j)
                            {
                                dateString += ":";
                            }
                        }
                    }
                }

            } catch (Exception e) {

            }

            Natives.glthread_GetUploadImage(bytes, (int) bytes.length, absolutePath,
                    originalWidth, originalHeight, dateString);
        }
    }
}


