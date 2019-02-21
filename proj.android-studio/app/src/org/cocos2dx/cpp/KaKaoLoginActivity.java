package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.Toast;

import com.kakao.auth.ApiResponseCallback;
import com.kakao.auth.AuthType;
import com.kakao.auth.ErrorCode;
import com.kakao.auth.ISessionCallback;
import com.kakao.auth.Session;
import com.kakao.network.ErrorResult;
import com.kakao.usermgmt.UserManagement;
import com.kakao.usermgmt.callback.MeResponseCallback;
import com.kakao.usermgmt.response.model.UserProfile;
import com.kakao.util.exception.KakaoException;
import com.kakao.util.helper.log.Logger;

import java.util.Map;

/**
 * Created by simgyubin on 2017. 6. 20..
 */

public class KaKaoLoginActivity extends Activity {

    private SessionCallback callback;      //콜백 선언

    private static final String TAG = "KaKaoLoginActivity";
    private int mResultCode = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "onCreate");

        Intent intent = getIntent();

        if(intent != null )
        {
            String data = intent.getStringExtra("type");
            mResultCode = intent.getIntExtra("resultCode", 0);

            Log.d(TAG, "type : " + data);

            if(data.equals("signIn"))
            {

            }
            else if(data.equals("signOut"))
            {

            }
            else if(data.equals("revokeAccess"))
            {

            }
        }


        callback = new SessionCallback();                  // 이 두개의 함수 중요함
        Session.getCurrentSession().addCallback(callback);
        Session.getCurrentSession().open(AuthType.KAKAO_LOGIN_ALL, this);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (Session.getCurrentSession().handleActivityResult(requestCode, resultCode, data)) {
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Session.getCurrentSession().removeCallback(callback);
    }

    private void requestSignUp(final Map<String, String> properties) {
        UserManagement.requestSignup(new ApiResponseCallback<Long>() {
            @Override
            public void onNotSignedUp() {
            }

            @Override
            public void onSuccess(Long result) {

                requestMe();
            }

            @Override
            public void onFailure(ErrorResult errorResult) {

                String message = "UsermgmtResponseCallback : failure : " + errorResult;
                Log.d(TAG, "onFailure " + message);

                Intent intent = new Intent();
                intent.putExtra("isSuccess", false);
                setResult(mResultCode, intent);
                finish();
            }

            @Override
            public void onSessionClosed(ErrorResult errorResult) {

                Log.d(TAG, "onSessionClosed");

                Intent intent = new Intent();
                intent.putExtra("isSuccess", false);
                setResult(mResultCode, intent);
                finish();
            }
        }, properties);
    }

    /**
     * 사용자의 상태를 알아 보기 위해 me API 호출을 한다.
     */
    protected void requestMe() {
        UserManagement.requestMe(new MeResponseCallback() {
            @Override
            public void onFailure(ErrorResult errorResult) {

                Log.d(TAG, "onFailure failed to get user info. msg= errorResult");

                Intent intent = new Intent();
                intent.putExtra("isSuccess", false);
                setResult(mResultCode, intent);
                finish();
            }

            @Override
            public void onSessionClosed(ErrorResult errorResult) {

                Log.d(TAG, "onSessionClosed");

                Intent intent = new Intent();
                intent.putExtra("isSuccess", false);
                setResult(mResultCode, intent);
                finish();
            }

            @Override
            public void onSuccess(UserProfile userProfile) {

                Log.d(TAG, "onSuccess");

                Log.d(TAG, "onSuccess getId : " + userProfile.getId());
                Log.d(TAG, "onSuccess getNickname : " + userProfile.getNickname());
                Log.d(TAG, "onSuccess getEmail : " + userProfile.getEmail());

                Intent intent = new Intent();
                intent.putExtra("isSuccess", true);

                intent.putExtra("getId", String.valueOf(userProfile.getId()));
                intent.putExtra("getNickname", userProfile.getNickname());
                intent.putExtra("getEmail", userProfile.getEmail());

                setResult(mResultCode, intent);
                finish();
            }

            @Override
            public void onNotSignedUp() {

            }
        });
    }

    private class SessionCallback implements ISessionCallback {

        @Override
        public void onSessionOpened() {

            Log.d(TAG, "onSessionOpened");
            requestMe();

        }

        @Override
        public void onSessionOpenFailed(KakaoException exception) {
            if (exception != null) {
                Logger.e(exception);
            }
            Log.d(TAG, "onSessionOpenFailed");

            Intent intent = new Intent();
            intent.putExtra("isSuccess", false);
            setResult(mResultCode, intent);
            finish();
        }
    }

    protected void redirectSignupActivity() {       //세션 연결 성공 시 SignupActivity로 넘김
//        final Intent intent = new Intent(this, KakaoSignupActivity.class);
//        intent.setFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
//        startActivity(intent);
//        finish();
    }
}
