import java.rmi.Naming;
import java.util.Scanner;

public class Client implements Runnable {
    private final int a, b;

    public Client(int a, int b) {
        this.a = a;
        this.b = b;
    }

    public void run() {
        try {
            // Get the current thread ID
            long threadId = Thread.currentThread().getId();
            System.out.println("\n\nThread ID: " + threadId);  // Print the thread ID

            String url = "rmi://localhost/Server";
            ServerInterface serverInterface = (ServerInterface) Naming.lookup(url);

            // Perform operations
            System.out.println("Addition: " + serverInterface.add(a, b));
            System.out.println("Subtraction: " + serverInterface.sub(a, b));
            System.out.println("Multiplication: " + serverInterface.mul(a, b));
            System.out.println("Division: " + serverInterface.div(a, b));

        } catch (Exception e) {
            System.out.println("Client Error: " + e);
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        try {
            // Input numbers
            System.out.print("Enter Num 1 >> ");
            int a = sc.nextInt();

            System.out.print("Enter Num 2 >> ");
            int b = sc.nextInt();

            System.out.print("Enter number of threads to run: ");
            int t = sc.nextInt();

            // Start multiple threads
            for (int i = 0; i < t; i++) {
                Thread clientThread = new Thread(new Client(a, b));
                clientThread.start();  // Start the thread
                clientThread.join();   // Wait for this thread to finish before starting the next
            }
        } catch (Exception e) {
            System.out.println("Client Error: " + e);
        } finally {
            sc.close();
        }
    }
}