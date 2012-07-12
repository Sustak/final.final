package finale;

import finale.utils.Date;

/**
 * Class Cashflow provides fundamental information about a cashflow.
 * The class is immutable, hence classes are free to return references to
 * instances used internally without fear of the Cashflow object being
 * modified.
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */

public class Cashflow {

    private Date date;
    private double amount;

    /**
     * Default constructor for Cashflow class.
     * @param date
     * @param amount
     */
    public Cashflow( Date date, double amount ) {
        this.date = date;
        this.amount = amount;
    }

    /**
     * @return Returns the date on which cashflow takes place.
     */
    public Date getDate() {
        // we can return the date directly, because class Date is immutable
        return date;
    }

    /**
     * @return Returns the amount of the cashflow.
     */
    public double getAmount() {
        return amount;
    }
}
