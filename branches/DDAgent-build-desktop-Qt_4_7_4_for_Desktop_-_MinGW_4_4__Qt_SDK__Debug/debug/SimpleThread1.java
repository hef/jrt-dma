public class SimpleThread1 {

     static MyThread t;

 

     public static void main(String args[]) throws Throwable{

 

       t = new MyThread();

       System.out.println("Creating and running 10 threads...");

       Thread thr1 = new Thread(t,"MyThread"+1);
       thr1.start();
       //Thread thr2 = new Thread(t,"MyThread"+2);
       //thr2.start();
              

      //for(int i = 0; i < 5; i++) {

         // Thread thr = new Thread(t,"MyThread"+i);

          //thr.start();

 

       try {

            thr1.join();
           // thr2.join();
            

        } catch (Throwable t) {

       }

     //}

   }

  }

 

 class MyThread implements Runnable {

 

    Thread t;

 

    public MyThread() {

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