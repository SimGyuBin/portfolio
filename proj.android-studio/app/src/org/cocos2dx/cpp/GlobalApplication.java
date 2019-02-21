package org.cocos2dx.cpp;

import android.app.Application;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.os.StrictMode;
import android.util.Log;

import com.kakao.auth.ApprovalType;
import com.kakao.auth.AuthType;
import com.kakao.auth.IApplicationConfig;
import com.kakao.auth.ISessionConfig;
import com.kakao.auth.KakaoAdapter;
import com.kakao.auth.KakaoSDK;
import com.onesignal.OSNotification;
import com.onesignal.OSNotificationAction;
import com.onesignal.OSNotificationOpenResult;
import com.onesignal.OSNotificationPayload;
import com.onesignal.OneSignal;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by simgyubin on 2017. 6. 20..
 */

public class GlobalApplication extends Application {

    private static volatile GlobalApplication instance = null;

    private static class KakaoSDKAdapter extends KakaoAdapter {
        /**
         * Session Config에 대해서는 default값들이 존재한다.
         * 필요한 상황에서만 override해서 사용하면 됨.
         * @return Session의 설정값.
         */
        @Override
        public ISessionConfig getSessionConfig() {
            return new ISessionConfig() {
                @Override
                public AuthType[] getAuthTypes() {
                    return new AuthType[] {AuthType.KAKAO_TALK};
                }

                @Override
                public boolean isUsingWebviewTimer() {
                    return false;
                }

                @Override
                public boolean isSecureMode() {
                    return false;
                }

                @Override
                public ApprovalType getApprovalType() {
                    return ApprovalType.INDIVIDUAL;
                }

                @Override
                public boolean isSaveFormData() {
                    return true;
                }
            };
        }

        @Override
        public IApplicationConfig getApplicationConfig() {
            return new IApplicationConfig() {
                @Override
                public Context getApplicationContext() {
                    return GlobalApplication.getGlobalApplicationContext();
                }
            };
        }
    }

    public static GlobalApplication getGlobalApplicationContext() {

        if(instance == null)
            throw new IllegalStateException("this application does not inherit com.kakao.GlobalApplication");
        return instance;
    }

    public static class NotificationReceivedHandler implements OneSignal.NotificationReceivedHandler {
        @Override
        public void notificationReceived(OSNotification notification) {

            Log.d("OneSignal", "notificationReceived");

            OSNotificationPayload payload = notification.payload;

            String strMessage = "";
            String strTitle = "";
            String push_event = "";

            if(payload != null)
            {
                JSONObject data = notification.payload.additionalData;

                strMessage = payload.body;
                strTitle = payload.title;

                if (data != null)
                {
                    push_event = data.optString("push_event", null);

                    if (push_event != null)
                    {
                        Log.d("OneSignal", "push_event set with value: " + push_event);
                    }
                }
            }

            if(Natives.sharedDelegate().mainActivity != null)
            {
                // 앱 활성화 상태
                // 상단 상태바에 푸시 아이콘이 등록 되지 않도록 삭제 해주자
                int id = notification.androidNotificationId;
                OneSignal.cancelNotification(id);
//                OneSignal.clearOneSignalNotifications();
                Natives.glthread_OneSignalHandleNotification(strMessage,strTitle,push_event, true);
            }
            else
            {
                // 앱 비활성화 상태
            }
        }
    }

    public static class NotificationOpenedHandler implements OneSignal.NotificationOpenedHandler {
        // This fires when a notification is opened by tapping on it.

        private Application application;

        public NotificationOpenedHandler(Application application) {
            this.application = application;
        }

        @Override
        public void notificationOpened(OSNotificationOpenResult result) {

            Log.d("OneSignal", "notificationOpened");

            OSNotificationPayload payload = result.notification.payload;

            String strMessage = "";
            String strTitle = "";
            String push_event = "";

            if(payload != null)
            {
                JSONObject data = result.notification.payload.additionalData;

                strMessage = payload.body;
                strTitle = payload.title;

                if (data != null)
                {
                    push_event = data.optString("push_event", null);

                    if (push_event != null)
                    {
                        Log.d("OneSignal", "push_eventset with value: " + push_event);
                    }
                }
            }

            // The following can be used to open an Activity of your choice.
            // Replace - getApplicationContext() - with any Android Context.

            if(Natives.sharedDelegate().mainActivity != null)
            {
                // 앱 활성화 상태
                Natives.glthread_OneSignalHandleNotification(strMessage,strTitle,push_event, true);
            }
            else
            {
                if(Natives.sharedDelegate().mbln_IntroFinish == false)
                {
                    // 앱 비활성화 상태
                    Intent intent = new Intent(application, AppActivity.class);
                    intent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_NEW_TASK);

                    intent.putExtra("type", "push");
                    intent.putExtra("message",strMessage);
                    intent.putExtra("title",strTitle);
                    intent.putExtra("push_event",push_event);

                    application.startActivity(intent);
                }
                else
                {
                    Natives.glthread_OneSignalHandleNotification(strMessage,strTitle,push_event, true);
                }
            }

            // Add the following to your AndroidManifest.xml to prevent the launching of your main Activity
            //   if you are calling startActivity above.
     /*
        <application ...>
          <meta-data android:name="com.onesignal.NotificationOpened.DEFAULT" android:value="DISABLE" />
        </application>
     */
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        KakaoSDK.init(new KakaoSDKAdapter());

        OneSignal.startInit(this)
//                .autoPromptLocation(true)
                .inFocusDisplaying(OneSignal.OSInFocusDisplayOption.Notification)
                .setNotificationReceivedHandler(new NotificationReceivedHandler())
                .setNotificationOpenedHandler(new NotificationOpenedHandler(this))
                .init();

        Log.d("OneSignal", "startInit ");
    }

    /**
     * 애플리케이션 종료시 singleton 어플리케이션 객체 초기화한다.
     */
    @Override
    public void onTerminate() {
        super.onTerminate();
        instance = null;
    }
}

