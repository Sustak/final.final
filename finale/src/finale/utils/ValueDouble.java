package finale.utils;

import static finale.utils.Math.*;

/**
 *
 * @author Marek Šesták
 * @version 1.0
 */
public class ValueDouble extends Value {

    public ValueDouble() {
        value = 0.0;
    }

    public ValueDouble( double value ) {
        this.value = value;
    }

    public ValueDouble( ValueListener listener ) {
        subscribe( listener );
    }

    public boolean toBoolean() {
        if (Round(toDouble(), 0) == 0) {
            return false;
        } else {
            return true;
        }
    }

    public Date toDate() {
        return Date.getValueOf(toInt());
    }

    public double toDouble() {
        return value;
    }

    public int toInt() {
        return (int) toDouble();
    }

    public String toString() {
        return Double.toString(toDouble());
    }

    public void set(double newValue) {
        value = newValue;
        super.updateSubscribers();
    }

    public void set(String newValue) {
        double dvalue = Double.valueOf(newValue);
        set( dvalue );
    }

    public void set(int newValue) {
        set((double)newValue);
    }

    public void set(Date newValue) {
        set((double)newValue.toInt());
    }

    public void set(boolean newValue) {
        if( newValue )
            set(1.0);
        else
            set(0.0);
    }

    private double value;
}
