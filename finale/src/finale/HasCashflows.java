package finale;

import finale.utils.Date;

/**
 * An object that implements the HasCashflows interface generates cashflows.
 * The cashflows can be returned one at a time using the <code>getNextCashflow</code>
 * function or as an array (functions <code>getCashflows</code>).
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public interface HasCashflows {

    /**
     * Function getNextCashflow returns the nearest cashflow the security 
     * generates after the settlement Date.
     * 
     * @param settlementDate 
     * 
     * @return Function returns the nearest cashflow the security generates after the
     * the provided settlement date. If the security generates no more cashflows
     * after the settlement date, <code>null</code> is returned.
     */
    public Cashflow getNextCashflow(Date settlementDate);

    /**
     * Function getCashflows returns an array containing all cashflows the
     * security generates.
     * @return
     * Function getCashflows returns an array containing all cashflows the
     * security generates. The returned array is a new copy of the objects
     * cashflow array, hence clients can modify the array without affecting
     * the security.
     */
    public Cashflow[] getCashflows();

    /**
     * Function getCashflows returns an array containing all cashflows the
     * security generates.
     *
     * @param settlementDate only cashflows with value date greater than
     * <code>settlementDate</code> will be returned
     *
     * @return
     * Function getCashflows returns an array containing all cashflows the
     * security generates. The returned array is a new copy of the objects
     * cashflow array, hence clients can modify the array without affecting
     * the security.
     */
    public Cashflow[] getCashflows(Date settlementDate);

}
