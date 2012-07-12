package finale;

import finale.utils.Value;
import finale.utils.ValueListener;
import java.util.HashMap;

/**
 *
 * @author Marek Šesták
 * @version 1.0
 */
public class Market implements ValueListener {

    private static HashMap<String,Market> cache = new HashMap<String, Market>();

    {
        // Market eur
    }

    String marketName;
    String currency;
    DaycountConvention mmktDaycountConvention = DaycountConvention.ACT_360;
    DaycountConvention bondDaycountConvention = DaycountConvention.ACT_ACT;
    BusinessDayConvention businessDayConvention = BusinessDayConvention.MODIFIED_FOLLOWING;
    
    public void priceQuoteUpdate(PriceQuote quote) {}

    public static Market getMarket( String name ) {

        return cache.get(name);

    }

    public void valueUpdate(Value value) {
        // TODO Market.valueUpdate
        throw new UnsupportedOperationException("Not supported yet.");
    }

}
