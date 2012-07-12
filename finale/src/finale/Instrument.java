package finale;

import finale.utils.Date;

/**
 * An object that implements interface <code>Instrument</code> has a market
 * value (<code>getMarketValue</code> function)
 * and can tell what kind of instrument is is.</p>
 * @see InstrumentType
 * @see PriceConvention
 * @author Marek Šesták <marek.sestak@gmail.com> 
 * @version 1.0
 */
public interface Instrument {

    /**
     * Function getMarketValue returns the market value the instrument has
     * depending on a value date and price, using the instrument's default
     * interpretation of the price.
     *
     * @param amount security's nominal amount
     * @param valueDate represents a date, for which the instrument is
     *          marked-to-market
     * @param price priced used to value the security
     *
     * @return Function returns total market value of the security, including
     * accrued interest for bonds, taking into account other factors (like
     * ex-coupon dates etc.) Price is interpreted in accordance with security's
     * default pricing convention, so {@link PriceConvention#Price} is used for
     * bonds, {@link PriceConvention#MoneyMarketYield} is used for most
     * treasury bills and certificates of deposit and so on.
     *
     * @throws IllegalArgumentException for invalid value dates, that is for
     * example those before the instrument has been issued or after the
     * security has matured.
     *
     * @see PriceConvention
     * @see InstrumentType
     */
    public double getMarketValue(double amount, Date valueDate, double price);

    /**
     * Function getMarketValue returns the market value the instrument has
     * depending on a value date and price.
     *
     * @param amount security's nominal amount
     * @param valueDate represents a date, for which the instrument is
     *          marked-to-market
     * @param price priced used to value the security
     * @param priceConvention tells the function how to interpret the price
     * (that is whether price, yield or some other form has been provided)
     *
     * @return Function returns total market value of the security, including
     * accrued interest for bonds, taking into account other factors (like
     * ex-coupon dates etc.) Price is interpreted in accordance with
     * parameter <code>priceConvention</code>.
     *
     * @throws IllegalArgumentException for invalid value dates, that is for
     * example those before the instrument has been issued or after the
     * security has matured.
     *
     * @see PriceConvention
     * @see InstrumentType
     */
    public double getMarketValue(double amount, Date valueDate, double price,
            PriceConvention priceConvention);

    /**
     * Function getInstrumentType returns instrument type of the current
     * instance of the object.
     *
     * @return Function return type of the current instance of the object.
     */
    public InstrumentType getInstrumentType();
}
