/**
 * Extend this class to manage printouts of classes that needs to print stack
 * traces. Useful for debugging. Use the method "print()" instead of the
 * "System.out.println()".
 */
public class ConsolePrinter{

    /** Decides if prints are allowed. This is public for easy access. */
    public boolean allowPrints = true;

    /**
     * Constructor. All classes that extends this class will get a console
     * message printed out when constructing
     */
    public ConsolePrinter() {
        print("Constructing");
    }

    /** This methods prints "[Class name] : */
    public void print() {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": ");
    }

    /** This methods prints "[Class name] : [data] */
    public void print(boolean data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(char data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(int data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(long data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(float data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(double data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(char[] data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(String data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

    /** This methods prints "[Class name] : [data] */
    public void print(Object data) {
        if (allowPrints) System.out.println(this.getClass().getSimpleName()
                        + ": " + data);
    }

}
