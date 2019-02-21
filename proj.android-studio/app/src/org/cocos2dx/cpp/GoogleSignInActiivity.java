package org.cocos2dx.cpp;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.common.api.Scope;
import com.google.android.gms.common.api.Status;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.GoogleAuthProvider;
import com.pixholic.pzle.R;

public class GoogleSignInActiivity extends FragmentActivity implements
        GoogleApiClient.OnConnectionFailedListener {

    private static final String TAG = "Cocos2dxActivity";
    private static final int RC_SIGN_IN = 9001;
    private static final int RC_SIGN_OUT = 9002;
    private ProgressDialog mProgressDialog;
    private int mResultCode = 0;


    // [START declare_auth]
    private FirebaseAuth mAuth;
    // [END declare_auth]

    private GoogleApiClient mGoogleApiClient;

    boolean mbln_signIn = false;
    boolean mbln_signOut = false;
    boolean mbln_revokeAccess = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        Log.d("Cocos2dxActivity","GoogleSignInActiivity onCreate");

        // [START config_signin]
        // Configure Google Sign In

        GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestIdToken(getString(R.string.default_web_client_id))
                .requestEmail()
                .requestProfile()
                .requestScopes(new Scope("https://picasaweb.google.com/data/"))
                .build();

        // [END config_signin]


        Log.d(TAG, "default_web_client_id : " + getString(R.string.default_web_client_id));

        mGoogleApiClient = new GoogleApiClient.Builder(this)
                .enableAutoManage(this, this)
                .addApi(Auth.GOOGLE_SIGN_IN_API, gso)
                .build();

        mAuth = FirebaseAuth.getInstance();

        Intent intent = getIntent();

        if(intent != null )
        {
            String data = intent.getStringExtra("type");
            mResultCode = intent.getIntExtra("resultCode", 0);

            Log.d(TAG, "type : " + data);

            if(data.equals("signIn"))
            {
                mbln_signIn = true;
            }
            else if(data.equals("signOut"))
            {
                mbln_signOut = true;
            }
            else if(data.equals("revokeAccess"))
            {
                mbln_revokeAccess = true;
            }
        }
    }

    @Override
    public void onStart() {
        super.onStart();

//        FirebaseUser currentUser = mAuth.getCurrentUser();

        if(mbln_signIn == true)
        {
            signIn();
        }
        else if(mbln_signOut == true)
        {
            signOut();
        }
        else if(mbln_revokeAccess == true)
        {
            revokeAccess();
        }

    }

    // [START onActivityResult]
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        // Result returned from launching the Intent from GoogleSignInApi.getSignInIntent(...);
        if (requestCode == RC_SIGN_IN) {

            Log.d(TAG, "GoogleSignInActiivity onActivityResult");

            GoogleSignInResult result = Auth.GoogleSignInApi.getSignInResultFromIntent(data);

            Log.d(TAG, "GoogleSignInActiivity onActivityResult : " + result.isSuccess());

            if (result.isSuccess()) {
                // Google Sign In was successful, authenticate with Firebase
                GoogleSignInAccount account = result.getSignInAccount();
                firebaseAuthWithGoogle(account);
            } else {
                // Google Sign In failed, update UI appropriately
                // [START_EXCLUDE]

                Intent intent = new Intent();
                intent.putExtra("isSuccess", result.isSuccess());
                setResult(mResultCode, intent);
                finish();

                // [END_EXCLUDE]
            }
        }
        else if(requestCode == RC_SIGN_OUT)
        {
            Intent intent = new Intent();
            intent.putExtra("isSuccess", false);
            setResult(mResultCode, intent);
            finish();
        }

    }
    // [END onActivityResult]

    private void firebaseAuthWithGoogle(GoogleSignInAccount acct) {
        Log.d(TAG, "firebaseAuthWithGoogle:" + acct.getId());
        // [START_EXCLUDE silent]
        showProgressDialog();
        // [END_EXCLUDE]

        final GoogleSignInAccount googleSignInAccount = acct;

        AuthCredential credential = GoogleAuthProvider.getCredential(acct.getIdToken(), null);
        mAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {

                        Intent intent = new Intent();
                        intent.putExtra("isSuccess", task.isSuccessful());

                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            Log.d(TAG, "signInWithCredential:success");
                            FirebaseUser user = mAuth.getCurrentUser();

                            Log.d(TAG, "handleSignInResult 2 :" + user.getDisplayName());
                            Log.d(TAG, "handleSignInResult result 2 :" + googleSignInAccount.getId() +","+googleSignInAccount.getEmail()+","+googleSignInAccount.getIdToken()+","+
                                    googleSignInAccount.getServerAuthCode());
                            Log.d(TAG, "handleSignInResult acct 2 :" + user);

                            intent.putExtra("getDisplayName", user.getDisplayName());
                            intent.putExtra("getId", googleSignInAccount.getId());
                            intent.putExtra("getEmail", user.getEmail());
                            intent.putExtra("getIdToken", googleSignInAccount.getIdToken());


                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w(TAG, "signInWithCredential:failure", task.getException());

                        }

                        // [START_EXCLUDE]
                        hideProgressDialog();

                        setResult(mResultCode, intent);
                        finish();

                        // [END_EXCLUDE]
                    }
                });
    }
    // [END auth_with_google]

    // [START signIn]
    private void signIn() {
        Intent signInIntent = Auth.GoogleSignInApi.getSignInIntent(mGoogleApiClient);
        startActivityForResult(signInIntent, RC_SIGN_IN);
    }
    // [END signIn]

    // [START signOut]
    private void signOut() {

        Log.d(TAG, "signOut");

        mAuth.signOut();

        // Google sign out
        Auth.GoogleSignInApi.signOut(mGoogleApiClient).setResultCallback(
                new ResultCallback<Status>() {
                    @Override
                    public void onResult(@NonNull Status status) {

                        Intent intent = new Intent();
                        intent.putExtra("key", "signOut");
                        setResult(mResultCode, intent);
                        finish();
                    }
                });
    }
    // [END signOut]

    // [START revokeAccess]
    private void revokeAccess() {
        // Firebase sign out
        mAuth.signOut();

        // Google revoke access
        Auth.GoogleSignInApi.revokeAccess(mGoogleApiClient).setResultCallback(
                new ResultCallback<Status>() {
                    @Override
                    public void onResult(@NonNull Status status) {

                        Log.d(TAG, "revokeAccess");
                        Intent intent = new Intent();
                        intent.putExtra("key", "revokeAccess");
                        setResult(mResultCode, intent);
                        finish();
                    }
                });
    }
    // [END revokeAccess]

    @Override
    public void onConnectionFailed(ConnectionResult connectionResult) {
        // An unresolvable error has occurred and Google APIs (including Sign-In) will not
        // be available.
        Log.d(TAG, "onConnectionFailed:" + connectionResult);
    }

    private void showProgressDialog() {
        if (mProgressDialog == null) {
            mProgressDialog = new ProgressDialog(this);
            mProgressDialog.setMessage("LOADING");
            mProgressDialog.setIndeterminate(true);
        }

        mProgressDialog.show();
    }

    private void hideProgressDialog() {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            mProgressDialog.hide();
        }
    }
}
