/**
 * This class can be extended and used to manage printouts of classes who needs
 * to print stack traces. Useful for debugging. Use the method "print()"
 */
public class ConsolePrinter{
    
    /** Decides if prints are allowed*/
    public boolean allowPrints = true;

    public void print() {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": ");
    }

    public void print(boolean data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(char data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(int data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(long data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(float data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(double data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(char[] data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(String data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    public void print(Object data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

}
