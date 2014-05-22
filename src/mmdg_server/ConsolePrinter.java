package mmdg_server;
/**
 * Extend this class to manage printouts of classes that needs to print stack
 * traces. Useful for debugging. Use the method "print()" instead of the
 * "System.out.println()".
 */
public class ConsolePrinter{

    /** Decides if prints are allowed. This is public for easy access. */
    protected boolean allowPrints = false;

    
    /**
     * Constructor. All classes that extends this class will get a console
     * message printed out when constructing
     */
    protected ConsolePrinter() {
        print("Constructing");
    }
    
    /**
     * Setter for allowPrints
     * @param allowPrints specifies specifies the new value of allowPrints
     */
    public void setAllowPrints(boolean allowPrints){
        this.allowPrints = allowPrints;
    }
    

    /** This methods prints "[Class name] : */
    protected void print() {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": ");
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(boolean data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(char data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(int data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(long data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(float data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(double data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(char[] data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": "
                            + data.toString());
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(String data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }

    /** This methods prints "[Class name] : [data] */
    protected void print(Object data) {
        if (allowPrints) {
            System.out.println(this.getClass().getSimpleName() + ": " + data);
        }
    }
}
