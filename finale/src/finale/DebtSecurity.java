package finale;

import finale.utils.Date;

/**
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public interface DebtSecurity {

    public DebtSecurityClass getDebtSecurityClass();

    public double getYTM(Date settlementDate, double price);

    public double getConventionalYield(Date settlementDate, double price);

    public double getConventionalPrice(Date settlementDate, double price);

    public double getPriceFromYTM(Date settlementDate, double yield);

    public double getAccruedInterest(Date settlementDate);

	public double getDuration(Date settlementDate, double price );

	public double getMDuration(Date settlementDate, double price );

}
