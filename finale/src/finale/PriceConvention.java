package finale;

/** PriceConvention enumaration defines how price in various function calls
 * should be interpreted, that is if it should be treated as price, yield or else.
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public enum PriceConvention {

    /**
     * Price is represented as <b>clean price</b>
     * expressed in percentage points; that is price is for example
     * 100 (so called "par"), 99.9 etc.
     */
    Price,
    /**
     * Price is represented as annual <b>yield</b> standard for the given instrument
     * (or sometimes also called conventional yield) passed in a decimal format.
     * Hence, 3% yield is for example
     * passed as 0.03. Conventional yield is different for various types of
     * instruments, so for example for bonds convention yield is the same as
     * yield to maturity, for certificates of deposits it is the {@link #MoneyMarketYield}
     * and so on.
     */
    Yield,
    /**
     * Price is represented as annul <b>yield to maturity</b>, that is
     * either annually or semi-annually compounded rate, at which all
     * instrument's future cashflows must be present-valued in order to
     * make their sum equal to the current market value of the instrument. </p>
     * This is a yield most typical for bonds (see {@link InstrumentType#Bond}).
     */
    YieldToMaturity,
    /**
     * Price is represented as <b>money market yield</b>,
     * that is a simple (not compounded) yield using typical
     * money market daycount convention (in most markets {@link DaycountConvention#ACT_360}).
     */
    MoneyMarketYield,
    /**
     * Price is represented as <b>discount rate</b>,
     * that is an annualized rate that is equivelent to the difference between
     * the face value of the security and the current market value.</p>
     * Discount is typically used for U.S. treasury bills.
     */
    Discount,
    /**
     * Price is represented as the market value itself. </p>
     * Although this doesn't look a very useful tool, this type of price
     * has been added to support less standard derivatives like swaps and so on.
     * In this case, price can be both positive and negative representing
     * a position in profit or in loss.
     */
    Absolute
}
