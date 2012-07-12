package finale.utils;

import java.util.Vector;

/**
 *
 * @author Marek Šesták
 * @version 1.0
 */
public abstract class Value {

    private Vector<ValueListener> subscribers;

    public void subscribe(ValueListener listener) {
        if (subscribers == null) {
            subscribers = new Vector<ValueListener>(4, 4);
            subscribers.add(listener);
        } else {
            if (subscribers.contains(listener) == false) {
                subscribers.add(listener);
            }
        }
    }

    public void unsubscribe(ValueListener listener) {
        subscribers.remove(listener);
    }

    protected void updateSubscribers() {
        if( subscribers!=null ) {
            for( ValueListener listener : subscribers ) {
                listener.valueUpdate(this);
            }
        }
    }

    public abstract double toDouble();

    public abstract String toString();

    public abstract int toInt();

    public abstract Date toDate();

    public abstract boolean toBoolean();

    public abstract void set(double newValue);

    public abstract void set(String newValue);

    public abstract void set(int newValue);

    public abstract void set(Date newValue);

    public abstract void set(boolean newValue);
}