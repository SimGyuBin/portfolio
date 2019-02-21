package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;

public class ImagePickerView {

    public String imageName;
    private Uri imageUri;

    public static final int IMAGE_MAX_SIZE 	= 160;
    public static final int IMAGE_QUALITY 	= 80;			// compressionQuality (0:최대압축 ~ 100:최소압축)

    public static final int PICKER_CAMERA 	= 0;
    public static final int PICKER_GALLERY 	= 1;
    public static final int PICKER_CROP		= 2;


    public ImagePickerView()
    {
        this.imageName = null;
        this.imageUri = null;
    }



    // 카메라 or 사진첩 모드 선택화면 등장 (일단 IOS에 맞춰서 갤러리만 나오도록 조치)
    public void selectMode()
    {
//		showCamera();
        showGallery();
    }


    // 카메라 구동
    public void showCamera()
    {
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);

        // 임시로 사용할 파일의 경로&이름 생성
        imageUri = Uri.fromFile(new File(Natives.getLibraryPath(), "mypic_temp_" + String.valueOf(System.currentTimeMillis()) + ".jpg"));
        intent.putExtra(android.provider.MediaStore.EXTRA_OUTPUT, imageUri);

        // 특정기기에서 사진을 저장못하는 문제가 있어 다음을 주석처리 합니다.
        //intent.putExtra("return-data", true);
        Natives.sharedDelegate().mainActivity.startActivityForResult(intent, PICKER_CAMERA);
    }


    // 사진첩 열기
    public void showGallery()
    {
        Intent intent = new Intent(Intent.ACTION_PICK);
        intent.setType(android.provider.MediaStore.Images.Media.CONTENT_TYPE);
        Natives.sharedDelegate().mainActivity.startActivityForResult(intent, PICKER_GALLERY);
    }



    // 인텐트 콜백 처리
    public void onResult(int requestCode, Intent intent)
    {
        switch (requestCode)
        {
            // 앨범에서 가져올 때
            case ImagePickerView.PICKER_GALLERY :
                imageUri = intent.getData();		// PICKER_CAMERA 와 동일한 로직을 타므로 break를 넣지 않는다


                // 사진을 바로 찍어서 가져올 때
            case ImagePickerView.PICKER_CAMERA:
                Intent cropIntent = new Intent("com.android.camera.action.CROP");
                cropIntent.setDataAndType(imageUri, "image/*");
                cropIntent.putExtra("outputX", ImagePickerView.IMAGE_MAX_SIZE);		// 크롭된 이미지를 해당 비트맵 크기로 가져옴
                cropIntent.putExtra("outputY", ImagePickerView.IMAGE_MAX_SIZE);		// 크롭된 이미지를 해당 비트맵 크기로 가져옴
                cropIntent.putExtra("aspectX", 1);									// 이미지 영역 가로 비율
                cropIntent.putExtra("aspectY", 1);									// 이미지 영역 세로 비율
                cropIntent.putExtra("scale", true);									// default = true 라서 안써줘도 될 듯.
                cropIntent.putExtra("return-data", true);							// 크롭한 이미지를 Bundle을 통해 Bitmap으로 받아 사용할 수 있게 해줌.
//	        cropIntent.putExtra("output", imagePickerView.imageUri);			// 해당 경로로 사진을 저장해버림. (return-data와 동시에 사용하면 안됨.)
                Natives.sharedDelegate().mainActivity.startActivityForResult(cropIntent, PICKER_CROP);
                break;


            // 사진편집 후 가져올 때
            case ImagePickerView.PICKER_CROP:
                final Bundle extras = intent.getExtras();
                if (extras != null)
                {
                    Bitmap bitmap = extras.getParcelable("data");

                    // 라이브러리 폴더에 저장한다
                    File file = new File(Natives.getLibraryPath() + imageName);		// 경로&파일명 지정
                    FileOutputStream out = null;

                    try
                    {
                        file.createNewFile();
                        out = new FileOutputStream(file);

                        if (bitmap.compress(CompressFormat.JPEG, IMAGE_QUALITY, out))		// 압축하여 저장한다
                        {
                            Natives.glthread_SuccessPicture();	// 저장성공 알리기
                        }
                        else
                        {
                            // 저장실패 팝업 띄워주기 (단순 저장 실패)
                            new AlertDialog.Builder(Natives.sharedDelegate().mainActivity)
                                    .setTitle("Error")
                                    .setMessage("Unable to save image to Photo Album.")
                                    .setCancelable(false)
                                    .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                        @Override
                                        public void onClick(DialogInterface dialog, int whichButton)
                                        {
                                            Natives.glthread_CancelPicture();	// 저장실패 알리기
                                        } })
                                    .show();
                        }
                    }
                    catch (Exception e)
                    {
                        // 저장실패 팝업 띄워주기 (SD카드가 없어서 생기는 문제?)
                        new AlertDialog.Builder(Natives.sharedDelegate().mainActivity)
                                .setTitle("Error")
                                .setMessage("Unable to save image to Photo Album.")
                                .setCancelable(false)
                                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int whichButton)
                                    {
                                        Natives.glthread_CancelPicture();	// 저장실패 알리기
                                    } })
                                .show();

                        e.printStackTrace();
                    }
                    finally
                    {
                        try
                        {
                            out.close();
                        }
                        catch (NullPointerException e)
                        {
                            e.printStackTrace();
                        }
                        catch (IOException e)
                        {
                            e.printStackTrace();
                        }
                    }
                }

                // 임시 파일 삭제
                File f = new File(imageUri.getPath());
                if (f.exists())
                {
                    f.delete();
                }
                break;
        }
    }
}
