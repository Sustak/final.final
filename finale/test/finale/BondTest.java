/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package finale;

import finale.utils.Date;
import finale.db.DebtSecurityDB;
import finale.db.FieldType;
import finale.db.SimpleTable;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Marek Sestak <marek.sestak@gmail.com>
 */
public class BondTest {

    public static DebtSecurityDB db = new DebtSecurityDB();

    @BeforeClass
    public static void setUpClass() throws Exception {
        System.out.println("reading bonds database.csv");
        db.ImportCSV("bonds database.csv");
    }

    public BondTest() {
    }

    @Test
    public void TestComplete() {

        SimpleTable data = new SimpleTable();
        data.addColumn("ISIN", FieldType.String);
        data.addColumn("Ticker", FieldType.String);
        data.addColumn("Maturity", FieldType.Date);
        data.addColumn("Coupon", FieldType.Double);
        data.addColumn("Issue Date", FieldType.Date);
        data.addColumn("Interest Accrues From", FieldType.Date);
        data.addColumn("First Coupon Date", FieldType.Date);
        data.addColumn("Redemption", FieldType.Double);
        data.addColumn("Currency", FieldType.String);
        data.addColumn("Frequency", FieldType.Integer);
        data.addColumn("Daycount Convention", FieldType.Integer);
        data.addColumn("Ex-Dividend Days", FieldType.Integer);
        data.addColumn("Settlement Date", FieldType.Date);
        data.addColumn("Price", FieldType.Double);
        data.addColumn("Yield", FieldType.Double);
        data.addColumn("Accrued Interest", FieldType.Double);
        data.addColumn("MDuration", FieldType.Double);
        data.addColumn("Price Recalculated", FieldType.Double);

        data.appendFromCSVFile("bond test data.csv", true);

        int nrows = data.getNRows();
        int colISIN = data.getColumnIndex("ISIN");
        int colTicker = data.getColumnIndex("Ticker");
        int colMaturity = data.getColumnIndex("Maturity");
        int colCoupon = data.getColumnIndex("Coupon");
        int colIssueDate = data.getColumnIndex("Issue Date");
        int colInterestAccruesFrom = data.getColumnIndex("Interest Accrues From");
        int colFirstCouponDate = data.getColumnIndex("First Coupon Date");
        int colRedemption = data.getColumnIndex("Redemption");
        int colCurrency = data.getColumnIndex("Currency");
        int colFrequency = data.getColumnIndex("Frequency");
        int colDaycountConvention = data.getColumnIndex("Daycount Convention");
        int colExDividendDays = data.getColumnIndex("Ex-Dividend Days");
        int colSettle = data.getColumnIndex("Settlement Date");
        int colPrice = data.getColumnIndex("Price");
        int colYield = data.getColumnIndex("Yield");
        int colAccruedInterest = data.getColumnIndex("Accrued Interest");
        int colMDuration = data.getColumnIndex("MDuration");
        int colPriceRecalculated = data.getColumnIndex("Price Recalculated");

        int nfails = 0;

        for (int i = 0; i < nrows; i++) {
            String isin = data.getAsString(i, colISIN);

            Date settlement = data.getAsDate(i, colSettle);
            double price = data.getAsDouble(i, colPrice);
            double priceRecalculated = data.getAsDouble(i, colPriceRecalculated);
            double yield = data.getAsDouble(i, colYield);
            double accrued = data.getAsDouble(i, colAccruedInterest);
            double mdur = data.getAsDouble(i, colMDuration);

            String ticker = data.getAsString(i, colTicker);
            Date maturity = data.getAsDate(i, colMaturity);
            Date issued = data.getAsDate(i, colIssueDate);
            Date accruesFrom = data.getAsDate(i, colInterestAccruesFrom);
            Date firstCoupon = data.getAsDate(i, colFirstCouponDate);
            double coupon = data.getAsDouble(i, colCoupon);
            double redemption = data.getAsDouble(i, colRedemption);
            DaycountConvention basis = DaycountConvention.toDaycountConvention(data.
                    getAsInteger(i, colDaycountConvention));
            int frequency = data.getAsInteger(i, colFrequency);
            int exDividendDays = data.getAsInteger(i, colExDividendDays);

            String secStr = "";

            try {

                secStr = "<security not initialized, ISIN: " + isin + ">";
                DebtSecurity sec = Bond.getBond(new Bond.Builder().setTicker(ticker).
                        setMaturity(maturity).setCoupon(coupon).setIssueDate(issued).
                        setInterestAccruesFrom(accruesFrom).setFirstCouponDate(firstCoupon).
                        setFrequency(frequency).setDaycountConvention(basis).
                        setRedemption(redemption).setExDividendDays(exDividendDays));

                if (sec != null) {

                    secStr = sec.toString();
                    double calcYield = sec.getConventionalYield(settlement, price);
                    if (Math.abs(yield - calcYield * 100.0) >= 0.001) {
                        nfails++;
                        System.out.println(sec.toString() + ": yield expected " +
                                yield + ", got " + calcYield * 100.0);
                    }

                    double priceBack = sec.getConventionalPrice(settlement, calcYield);
                    double tolerance = 0.0001;

                    if (Math.abs(priceRecalculated - priceBack) > tolerance) {
                        nfails++;
                        System.out.println(sec.toString() + ": price expected " +
                                priceRecalculated + ", got " + priceBack);
                    }

                    double accruedCalc = sec.getAccruedInterest(settlement);
                    if (Math.abs(accruedCalc - accrued) > 0.00001) {
                        nfails++;
                        System.out.println("Accrued interest: expected " +
                                accrued + ", got " + accruedCalc + ", security: " +
                                sec.toString());
                    }

                    double mDurCalc = sec.getMDuration(settlement, price);
                    if (Math.abs(mDurCalc - mdur) > 0.001) {
                        nfails++;
                        System.out.println("MDuration: expected " +
                                mdur + ", got " + mDurCalc + ", security: " +
                                sec.toString());
                    }


                } else {
                    nfails++;
                    System.out.println("Failed to construct security ISIN:" + isin);
                }

            } catch (Exception e) {

                nfails++;
                System.out.println(secStr + ": " + e.getMessage());

            }
        }

        assertEquals(nfails, 0);
    }

    @Test
    public void TestGetPriceFromYTM() {

        System.out.println("Testing Bond.getPriceFromYTM");
        Bond bnd = Bond.getBond(new Bond.Builder() //
                .setMaturity(Date.getValueOf(15, 5, 2019)) //
                .setCoupon(0.03125) //
                .setFrequency(2) //
                .setDaycountConvention(DaycountConvention.ACT_ACT) //
                .setIssueDate(Date.getValueOf(15, 5, 2009)) //
                .setInterestAccruesFrom(Date.getValueOf(15, 5, 2009)) //
                .setFirstCouponDate(Date.getValueOf(15, 11, 2009)));

        Date settlement = Date.getValueOf(25, 6, 2009);
        double yield = 0.03160311;
        double priceCalculated = bnd.getPriceFromYTM(settlement, yield);
        double priceExpected = 99.70;

        boolean match;
        if (Math.abs(priceCalculated - priceExpected) < 0.000001) {
            match = true;
        } else {
            match = false;
        }

        assertTrue("getPriceFromYTM", match);

        double yieldCalculated = bnd.getYTM(settlement, priceExpected);
        if (Math.abs(yieldCalculated - yield) < 0.000001) {
            match = true;
        } else {
            match = false;
        }

        assertTrue("getYTM", match);
    }

    @Test
    public void TestBTPS() {

        System.out.println("Testing Bond.getPriceFromYTM");
        Bond bnd = Bond.getBond(new Bond.Builder() //
                .setTicker("BTPS") //
                .
                setMaturity(Date.getValueOf(1, 11, 2010)) //
                .
                setCoupon(0.055) //
                .
                setFrequency(2) //
                .
                setDaycountConvention(DaycountConvention.ACT_ACT) //
                .
                setIssueDate(Date.getValueOf(3, 4, 2000)) //
                .
                setInterestAccruesFrom(Date.getValueOf(1, 11, 1999)) //
                .
                setFirstCouponDate(Date.getValueOf(1, 5, 2000)));

        Date settlement = Date.getValueOf(4, 8, 2009);
        double price = 105.47;
        double yield = 0.01055933;
        double priceCalculated = bnd.getConventionalPrice(settlement, yield);

        boolean match;

        if (Math.abs(priceCalculated - price) < 0.000001) {
            match = true;
        } else {
            match = false;
        }

        assertTrue("Price calculation failed, extected " + price +
                " got: " + priceCalculated,
                match);

        double yieldCalculated = bnd.getConventionalYield(settlement, price);
        if (Math.abs(yieldCalculated - yield) < 0.0000001) {
            match = true;
        } else {
            match = false;
        }

        assertTrue("Yield calculation failed, extected " + yield +
                " got: " + yieldCalculated,
                match);
    }

    @Test
    public void TestGetCashflowIndex() {

        System.out.println("Testing Bond.getCashflowIndex...");

        Bond bnd = Bond.getBond((new Bond.Builder()).setCoupon(0.05).
                setMaturity(Date.getValueOf(1, 1, 2010)).
                setIssueDate(Date.getValueOf(15, 7, 2000)).
                setFirstCouponDate(Date.getValueOf(1, 1, 2002)).
                setFrequency(1));

        System.out.println(bnd.toString());
        Date dt;

        dt =
                Date.getValueOf(1, 1, 1990);
        assertEquals(dt.toString(), 0, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(1, 1, 2000);
        assertEquals(dt.toString(), 0, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(1, 1, 2001);
        assertEquals(dt.toString(), 0, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(1, 1, 2002);
        assertEquals(dt.toString(), 0, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(2, 1, 2002);
        assertEquals(dt.toString(), 1, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(31, 12, 2002);
        assertEquals(dt.toString(), 1, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(1, 1, 2003);
        assertEquals(dt.toString(), 1, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(1, 7, 2003);
        assertEquals(dt.toString(), 2, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(31, 12, 2003);
        assertEquals(dt.toString(), 2, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(1, 7, 2009);
        assertEquals(dt.toString(), 8, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(31, 12, 2009);
        assertEquals(dt.toString(), 8, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(1, 1, 2010);
        assertEquals(dt.toString(), 8, bnd.getCashflowIndex(dt));

        dt =
                Date.getValueOf(2, 1, 2011);
        assertEquals(dt.toString(), -1, bnd.getCashflowIndex(dt));


    }
}
