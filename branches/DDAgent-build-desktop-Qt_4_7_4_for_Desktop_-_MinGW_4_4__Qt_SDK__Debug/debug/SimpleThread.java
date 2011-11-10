 public class SimpleThread 
 {
     static MyThread t;
     public static void main(String args[]) throws Throwable
     {
       t = new MyThread();
	 //  System.out.println("\n----------------------------------------------------------------------------");
	   // System.out.println("-------------------JAVA PROG EXECUTION STARTS------------------------------");
	//	System.out.println("-----------------------------------------------------------------------------\n");
       System.out.println("Creating and running 10 threads...\n");
       for(int i = 0; i < 5; i++) 
       {
          Thread thr = new Thread(t,"MyThread"+i);
          thr.start(); 

       try 
	   {
            thr.join();
        } catch (Throwable t) {

       }

     }
	 
		//System.out.println("\n----------------------------------------------------------------------------");
//	    System.out.println("-------------------JAVA PROG EXECUTION ENDS------------------------------");
//		System.out.println("-----------------------------------------------------------------------------\n");

   }

  }

 

 class MyThread implements Runnable 
 {
    Thread t;

    public MyThread() 
    {

    }

 

    public void run() {

     /* NO-OP */

    try {

 

        "a".getBytes("ASCII");

        throwException();

        Thread.sleep(1000);

    } catch (java.lang.InterruptedException e){

        e.printStackTrace();

    } catch (Throwable t) {

   }

 }

 

 public void throwException() throws Throwable{

 

   throw new Exception("Thread Exception from MyThread");

   }

 } 