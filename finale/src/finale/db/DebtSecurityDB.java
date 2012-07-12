package finale.db;

import finale.Bond;
import finale.DebtSecurity;
import finale.DebtSecurityClass;
import java.util.HashMap;

/**
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class DebtSecurityDB implements SimpleTableListener {

    private HashMap<String, DebtSecurity> dbISIN = new HashMap<String, DebtSecurity>();
    private SimpleTable table = new SimpleTable(50000, 1000);
    private StringBuilder logger = new StringBuilder();
    private int colISIN;
    private int colTicker;
    private int colMaturity;
    private int colCoupon;
    private int colIssueDate;
    private int colInterestAccruesFrom;
    private int colFirstCouponDate;
    private int colRedemption;
    private int colCurrency;
    private int colFrequency;
    private int colConvention;

    public DebtSecurityDB() {
        int column = 0;
        table.addColumn("ISIN", FieldType.String);
        colISIN = column++;
        table.addColumn("Ticker", FieldType.String);
        colTicker = column++;
        table.addColumn("Maturity", FieldType.Date);
        colMaturity = column++;
        table.addColumn("Coupon", FieldType.Double);
        colCoupon = column++;
        table.addColumn("Issue Date", FieldType.Date);
        colIssueDate = column++;
        table.addColumn("Interest Accrues From", FieldType.Date);
        colInterestAccruesFrom = column++;
        table.addColumn("First Coupon Date", FieldType.Date);
        colFirstCouponDate = column++;
        table.addColumn("Frequency", FieldType.Integer);
        colFrequency = column++;
        table.addColumn("Daycount convention", FieldType.String);
        colConvention = column++;
        table.addColumn("Currency", FieldType.String);
        colCurrency = column++;
        table.addColumn("Redemption", FieldType.Double);
        colRedemption = column++;

        table.setListener(this);
    }

    public int ImportCSV(String csvfile) {
        return table.appendFromCSVFile(csvfile, Boolean.TRUE);
    }

    public DebtSecurity getDebtSecurity(String ISIN) {
        return dbISIN.get(ISIN);
    }

    public void recordInserted(int rowIndex) {
        DebtSecurity sec = null;
        String isin = table.getAsString(rowIndex, colISIN);

        if (dbISIN.containsKey(isin)) {
//            logger.append("Multiple securities with identical ISIN " + isin +
//                    ", only first security included in the database");
            // TODO logger should be enabled
            return;
        }

        try {
            sec = Bond.getBond(new Bond.Builder().setTicker(table.getAsString(rowIndex, colTicker)).
                    setMaturity(table.getAsDate(rowIndex, colMaturity)).
                    setCoupon(table.getAsDouble(rowIndex, colCoupon)).
                    setIssueDate(table.getAsDate(rowIndex, colIssueDate)).
                    setInterestAccruesFrom(table.getAsDate(rowIndex, colInterestAccruesFrom)).
                    setFirstCouponDate(table.getAsDate(rowIndex, colFirstCouponDate)).
                    setFrequency(table.getAsInteger(rowIndex, colFrequency)).
                    setRedemption(table.getAsDouble(rowIndex, colRedemption)).
                    setDaycountConvention(table.getAsString(rowIndex, colConvention)));
        } catch (Exception e) {
//            logger.append("Failed to construct security " + isin + ": " + e.getMessage() + "\n");
            // TOTO logger should be enabled
        }

        dbISIN.put(isin, sec);
    }

    public void recordDeletion(int rowIndex) {
        String isin = table.getAsString(rowIndex, colISIN);
        dbISIN.remove(isin);
    }

    public void recordUpdated(int rowIndex) {
        String isin = table.getAsString(rowIndex, colISIN);
        DebtSecurity sec = dbISIN.get(isin);
        if (sec != null) {
            if (sec.getDebtSecurityClass() == DebtSecurityClass.Bond) {
                Bond.Builder builder =
                        new Bond.Builder().setMaturity(table.getAsDate(rowIndex, colMaturity)).
                        setCoupon(table.getAsDouble(rowIndex, colCoupon)).
                        setIssueDate(table.getAsDate(rowIndex, colIssueDate)).
                        setInterestAccruesFrom(table.getAsDate(rowIndex, colInterestAccruesFrom)).
                        setFirstCouponDate(table.getAsDate(rowIndex, colFirstCouponDate)).
                        setFrequency(table.getAsInteger(rowIndex, colFrequency)).
                        setRedemption(table.getAsDouble(rowIndex, colRedemption)).
                        setDaycountConvention(table.getAsString(rowIndex, colISIN));
                ((Bond) sec).updateBond(builder);
            }
        }
    }
}
