package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 * 
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondBTPS extends Bond {

    private static Calendar calendarBTPS = Calendar.getCalendar("TARGET");
    BondBTPS(Builder parameters) {
        super( parameters );
    }

    @Override
    public String getBondType() {
        return "Italian government bond (BTPS)";
    }

	@Override
	public void updateBond(Builder parameters)
	{
		parameters.compoundingFrequency = 1;
		super.updateBond(parameters);
	}

    @Override
    public Calendar getCalendar() {
        return calendarBTPS;
    }

    @Override
    public double getAccruedInterest( Date settlementDate ) {
        return Round(super.getAccruedInterest(settlementDate),5);
    }

    @Override
    public double getConventionalPriceUnrounded(Date settlementDate, double yield) {
        return getTruePrice(settlementDate, yield);
    }

    @Override
    public double getConventionalYieldUnrounded(Date settlementDate, double price) {
        return getTrueYield(settlementDate, price);
    }

}
