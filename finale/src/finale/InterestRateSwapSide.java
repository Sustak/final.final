package finale;

import finale.utils.Date;

/**
 *
 * @author Marek Sestak
 * @version 1.0
 */
public class InterestRateSwapSide {

    public static class Parameters {

        private FixedFloat fixedFloat;
        private Double nominal;
        private String currency;
        private Double couponRate;
        private Double firstCouponRate;
        private Date effectiveDate;
        private Date maturity;
        private Date firstPayment;
        private Date nextToLastPayment;
        private BusinessDayConvention businessDayAdjustment;
        private DaycountConvention daycountConvention;
        private PaymentFrequency payFrequency;
        private PaymentFrequency reset;
        private String index;
        private String discountCurve;

        /**
         * @return the fixedFloat
         */
        public FixedFloat getFixedFloat() {
            return fixedFloat;
        }

        /**
         * @param fixedFloat the fixedFloat to set
         */
        public Parameters setFixedFloat(FixedFloat fixedFloat) {
            this.fixedFloat = fixedFloat;
            return this;
        }

        /**
         * @return the nominal
         */
        public Double getNominal() {
            return nominal;
        }

        /**
         * @param nominal the nominal to set
         */
        public Parameters setNominal(Double nominal) {
            this.nominal = nominal;
            return this;
        }

        /**
         * @return the currency
         */
        public String getCurrency() {
            return currency;
        }

        /**
         * @param currency the currency to set
         */
        public Parameters setCurrency(String currency) {
            this.currency = currency;
            return this;
        }

        /**
         * @return the couponRate
         */
        public Double getCouponRate() {
            return couponRate;
        }

        /**
         * @param couponRate the couponRate to set
         */
        public Parameters setCouponRate(Double couponRate) {
            this.couponRate = couponRate;
            return this;
        }


        public Double getFirstCouponRate() {
            return firstCouponRate;
        }

        public Parameters setFirstCouponRate(Double firstCouponRate) {
            this.firstCouponRate = firstCouponRate;
            return this;
        }

        /**
         * @return the effective
         */
        public Date getEffectiveDate() {
            return effectiveDate;
        }

        /**
         * @param effective the effective to set
         */
        public Parameters setEffectiveDate(Date effective) {
            this.effectiveDate = effective;
            return this;
        }

        /**
         * @return the maturity
         */
        public Date getMaturity() {
            return maturity;
        }

        /**
         * @param maturity the maturity to set
         */
        public Parameters setMaturity(Date maturity) {
            this.maturity = maturity;
            return this;

        }

        /**
         * @return the firstPayment
         */
        public Date getFirstPayment() {
            return firstPayment;
        }

        /**
         * @param firstPayment the firstPayment to set
         */
        public Parameters setFirstPayment(Date firstPayment) {
            this.firstPayment = firstPayment;
            return this;

        }

        /**
         * @return the nextToLastPayment
         */
        public Date getNextToLastPayment() {
            return nextToLastPayment;
        }

        /**
         * @param nextToLastPayment the nextToLastPayment to set
         */
        public Parameters setNextToLastPayment(Date nextToLastPayment) {
            this.nextToLastPayment = nextToLastPayment;
            return this;

        }

        /**
         * @return the businessDayAdjustment
         */
        public BusinessDayConvention getBusinessDayAdjustment() {
            return businessDayAdjustment;
        }

        /**
         * @param businessDayAdjustment the businessDayAdjustment to set
         */
        public Parameters setBusinessDayAdjustment(BusinessDayConvention businessDayAdjustment) {
            this.businessDayAdjustment = businessDayAdjustment;
            return this;

        }

        /**
         * @return the daycountConvention
         */
        public DaycountConvention getDaycountConvention() {
            return daycountConvention;
        }

        /**
         * @param daycountConvention the daycountConvention to set
         */
        public Parameters setDaycountConvention(DaycountConvention daycountConvention) {
            this.daycountConvention = daycountConvention;
            return this;

        }

        /**
         * @return the payFrequency
         */
        public PaymentFrequency getPayFrequency() {
            return payFrequency;
        }

        /**
         * @param payFrequency the payFrequency to set
         */
        public Parameters setPayFrequency(PaymentFrequency payFrequency) {
            this.payFrequency = payFrequency;
            return this;

        }

        /**
         * @return the reset
         */
        public PaymentFrequency getReset() {
            return reset;
        }

        /**
         * @param reset the reset to set
         */
        public Parameters setReset(PaymentFrequency reset) {
            this.reset = reset;
            return this;

        }

        /**
         * @return the index
         */
        public String getIndex() {
            return index;
        }

        /**
         * @param index the index to set
         */
        public Parameters setIndex(String index) {
            this.index = index;
            return this;

        }

        /**
         * @return the discountCurve
         */
        public String getDiscountCurve() {
            return discountCurve;
        }

        /**
         * @param discountCurve the discountCurve to set
         */
        public Parameters setDiscountCurve(String discountCurve) {
            this.discountCurve = discountCurve;
            return this;

        }
    }

    private FixedFloat fixedFloat;
    private Double nominal;
    private String currency;
    private double couponRate;
    private double firstCouponRate;
    private Date effectiveDate;
    private Date maturity;
    private Date firstPayment;
    private Date nextToLastPayment;
    private BusinessDayConvention businessDayAdjustment;
    private DaycountConvention daycountConvention;
    private PaymentFrequency payFrequency;
    private PaymentFrequency reset;
    private String index;
    private String discountCurve;

    public BusinessDayConvention getBusinessDayAdjustment() {
        return businessDayAdjustment;
    }

    public double getCouponRate() {
        return couponRate;
    }

    public double getFirstCouponRate() {
        return firstCouponRate;
    }

    public String getCurrency() {
        return currency;
    }

    public DaycountConvention getDaycountConvention() {
        return daycountConvention;
    }

    public String getDiscountCurve() {
        return discountCurve;
    }

    public Date getEffectiveDate() {
        return effectiveDate;
    }

    public Date getFirstPayment() {
        return firstPayment;
    }

    public FixedFloat getFixedFloat() {
        return fixedFloat;
    }

    public String getIndex() {
        return index;
    }

    public Date getMaturity() {
        return maturity;
    }

    public Date getNextToLastPayment() {
        return nextToLastPayment;
    }

    public Double getNominal() {
        return nominal;
    }

    public PaymentFrequency getPayFrequency() {
        return payFrequency;
    }

    public PaymentFrequency getReset() {
        return reset;
    }


}
