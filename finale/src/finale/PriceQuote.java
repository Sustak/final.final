package finale;

import finale.utils.ValueDouble;
import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * @author Marek Šesták
 * @version 1.0
 */
public class PriceQuote extends ValueDouble {

    String name;
    String bloombergTicker;
    String bloombergKey;
    boolean set = false;

    private static HashMap<String, PriceQuote> cache = new HashMap<String, PriceQuote>();

    private PriceQuote( String name, String bloombergTicker,
            String bloombergKey, Market market ) {

        this.name = name;
        this.bloombergTicker = bloombergTicker;
        this.bloombergKey = bloombergKey;
        subscribe(market);
    }

    public static PriceQuote getPriceQuote( String name, String bloombergTicker,
            String bloombergKey, Market market ) {

        PriceQuote pq = cache.get(name);

        if( pq==null ) {
            pq = new PriceQuote(name, bloombergTicker, bloombergKey, market );
            cache.put(name, pq);
        }
        else {
            pq.subscribe(market);
        }

        return null;
    }

    public static PriceQuote getPriceQuote( String name ) {

        return cache.get(name);

    }

    public double getValue() {
        if (!set) {
            throw new RuntimeException("Value of " + name + " not set.");
        }
        return toDouble();
    }

    public void set(double newValue) {
        super.set(newValue);
        set = true;
    }

    public String getBloombergKey() {
        return bloombergKey;
    }

    public String getBloombergTicker() {
        return bloombergTicker;
    }

    public String getName() {
        return name;
    }
}
