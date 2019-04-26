/* Copyright (c) 2012-2019. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package org.simgrid.msg;
/** A barrier  implemented on top of SimGrid synchronization mechanisms.
 * You can use it exactly the same way that you use barriers,
 * but to handle the interactions between the processes within the simulation.
 *
 * Don't mix simgrid synchronization with Java native one, or it will deadlock!
 */
public class Barrier {
	private long bind; // The C object -- don't touch it

	public Barrier(int count) {
		init(count);
	}

	@Override
	protected void finalize() throws Throwable {
		nativeFinalize();
	}
	private native void nativeFinalize();
	private native void init(int count);
	public native void enter();

	/** Class initializer, to initialize various JNI stuff */
	public static native void nativeInit();
	static {
		org.simgrid.NativeLib.nativeInit();
		nativeInit();
	}
}


