package finale;

import finale.utils.Date;
import finale.Bond.Builder;
import static finale.utils.Math.*;

/**
 *
 * @author Marek Sestak <marek.sestak@gmail.com>
 * @version 1.0
 */
public class BondSPGB extends Bond {

    BondSPGB(Builder parameters) {
        super( parameters );
    }

    @Override
    public String getBondType() {
        return "Spanish government bond (SPGB)";
    }

    @Override
    public double getConventionalPriceUnrounded(Date settlementDate, double yield) {
		if( getCalendar().isWeekDay(maturity.getWeekDay()) )
			return super.getConventionalPriceUnrounded(settlementDate, yield);
		else
			return getTruePrice(settlementDate, yield);
    }

    @Override
    public double getConventionalYieldUnrounded(Date settlementDate, double price) {
		if( getCalendar().isWeekDay(maturity.getWeekDay()) )
			return super.getConventionalYieldUnrounded(settlementDate, price);
		else
			return getTrueYield(settlementDate, price);
    }

}
