package org.cocos2dx.cpp;

import com.bumptech.glide.Glide;
import com.bumptech.glide.GlideBuilder;
import com.bumptech.glide.load.DecodeFormat;
import com.bumptech.glide.load.engine.bitmap_recycle.LruBitmapPool;
import com.bumptech.glide.load.engine.cache.DiskLruCacheFactory;
import com.bumptech.glide.load.engine.cache.ExternalCacheDiskCacheFactory;
import com.bumptech.glide.load.engine.cache.InternalCacheDiskCacheFactory;
import com.bumptech.glide.load.engine.cache.LruResourceCache;
import com.bumptech.glide.load.engine.cache.MemorySizeCalculator;
import com.bumptech.glide.module.GlideModule;

import android.content.Context;
import android.os.Environment;

public class MyGlideModule implements GlideModule {
    private final int maxMemory = (int) (Runtime.getRuntime().maxMemory() / 1024);
    private final int cacheSize = maxMemory / 8;
    private final int DISK_CACHE_SIZE = 1024 * 1024 * 10;

    @Override
    public void applyOptions(Context context, GlideBuilder builder) {

//		  MemorySizeCalculator calculator = new MemorySizeCalculator( context );
//
//		  int defaultMemoryCacheSize = calculator.getMemoryCacheSize();
//		  int defaultBitmapPoolSize = calculator.getBitmapPoolSize();
//
//		  int customMemoryCacheSize = (int) (1.2 * defaultMemoryCacheSize);
//		  int customBitmapPoolSize = (int) (1.2 * defaultBitmapPoolSize);
//
//		  builder.setMemoryCache( new LruResourceCache( customMemoryCacheSize ) );
//		  builder.setBitmapPool( new LruBitmapPool( customBitmapPoolSize ) );
//		  builder.setDecodeFormat(DecodeFormat.PREFER_RGB_565);

//		  builder.setDiskCache(new ExternalCacheDiskCacheFactory(context, "cache", DISK_CACHE_SIZE))
        builder.setMemoryCache(new LruResourceCache(cacheSize))
                .setDecodeFormat(DecodeFormat.PREFER_RGB_565);

        // memory cache


        // disk cache
        // set size & external vs. internal
//		    int cacheSize100MegaBytes = 104857600;
//
//		    builder.setDiskCache(
//		            new InternalCacheDiskCacheFactory( context, cacheSize100MegaBytes ) );
//
//		    builder.setDiskCache(
//		            new ExternalCacheDiskCacheFactory( context, cacheSize100MegaBytes ) );
//
//		    // set custom location
//		    String downloadDirectoryPath = Environment.getDownloadCacheDirectory().getPath();
//
//		    builder.setDiskCache(
//		            new DiskLruCacheFactory( downloadDirectoryPath, cacheSize100MegaBytes ) );
    }

    @Override
    public void registerComponents(Context context, Glide glide) {

    }
}