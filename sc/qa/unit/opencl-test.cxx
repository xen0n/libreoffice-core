/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <sal/config.h>

#include <string_view>

#include <test/bootstrapfixture.hxx>

#include "helper/qahelper.hxx"

#include <docsh.hxx>
#include <document.hxx>
#include <formulagroup.hxx>

using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;

class ScOpenCLTest
    : public ScBootstrapFixture
{
public:
    ScOpenCLTest();

    /**
     * Try to auto-detect OpenCL device if one is available.
     *
     * @return true if a usable OpenCL device is found, false otherwise.
     */
    bool detectOpenCLDevice();

    /**
     * Turn on OpenCL group interpreter. Call this after the document is
     * loaded and before performing formula calculation.
     */
    void enableOpenCL();

    virtual void setUp() override;
    virtual void tearDown() override;

    void testSystematic();
    void testSharedFormulaXLS();
#if 0
    void testSharedFormulaXLSGroundWater();
    void testSharedFormulaXLSStockHistory();
#endif
    void testFinacialFormula();
    void testStatisticalFormulaFisher();
    void testStatisticalFormulaFisherInv();
    void testStatisticalFormulaGamma();
    void testFinacialFvscheduleFormula();
// this test has intermittent failures on OSX
#if !defined MACOSX
    void testFinacialIRRFormula();
#endif
    void testFinacialMIRRFormula();
    void testFinacialRateFormula();
    void testFinancialAccrintmFormula();
    void testFinancialAccrintFormula();
    void testCompilerHorizontal();
    void testCompilerNested();
    void testFinacialSLNFormula();
    void testStatisticalFormulaGammaLn();
    void testStatisticalFormulaGauss();
    void testStatisticalFormulaGeoMean();
    void testStatisticalFormulaHarMean();
    void testFinancialCoupdaybsFormula();
    void testFinacialDollardeFormula();
    void testCompilerString();
    void testCompilerInEq();
    void testFinacialDollarfrFormula();
    void testFinacialSYDFormula();
    void testStatisticalFormulaCorrel();
    void testFinancialCoupdaysFormula();
    void testFinancialCoupdaysncFormula();
    void testFinacialDISCFormula();
    void testFinacialINTRATEFormula();
    void testMathFormulaCos();
    void testMathFormulaCsc();
    void testStatisticalFormulaRsq();
    void testStatisticalFormulaPearson();
    void testStatisticalFormulaNegbinomdist();
    void testFinacialXNPVFormula();
    void testFinacialPriceMatFormula();
    void testFinacialFormulaReceived();
    void testFinancialFormulaCumipmt();
    void testFinancialFormulaCumprinc();
    void testFinacialRRIFormula();
    void testFinacialEFFECT_ADDFormula();
    void testFinacialNominalFormula();
    void testFinacialTBILLEQFormula();
    void testFinacialTBILLPRICEFormula();
    void testFinacialTBILLYIELDFormula();
    void testFinacialYIELDFormula();
    void testFinacialYIELDDISCFormula();
    void testFinacialYIELDMATFormula();
    void testFinacialPMTFormula();
    void testFinacialPPMTFormula();
    void testFinancialISPMTFormula();
    void testFinacialPriceFormula();
    void testFinancialDurationFormula();
    void testFinancialCoupnumFormula();
    void testMathFormulaSinh();
    void testMathFormulaAbs();
    void testFinacialPVFormula();
    void testMathFormulaSin();
    void testMathFormulaTan();
    void testMathFormulaTanH();
    void testStatisticalFormulaStandard();
    void testStatisticalFormulaWeibull();
    void testStatisticalFormulaMedian();
    void testFinancialDuration_ADDFormula();
    void testFinancialAmordegrcFormula();
    void testFinancialAmorlincFormula();
    void testFinancialDDBFormula();
    void testFinancialFVFormula();
    void testFinancialMDurationFormula();
    void testMathSumIfsFormula();
    void testFinancialVDBFormula();
    void testStatisticalFormulaKurt();
    void testFinacialNPERFormula();
    void testStatisticalFormulaNormdist();
    void testMathFormulaArcCos();
    void testMathFormulaSqrt();
    void testMathFormulaArcCosHyp();
    void testFinacialNPVFormula();
    void testStatisticalFormulaNormsdist();
    void testStatisticalFormulaNorminv();
    void testStatisticalFormulaNormsinv();
    void testStatisticalFormulaPermut();
    void testStatisticalFormulaPermutation();
    void testStatisticalFormulaPhi();
    void testFinancialIPMTFormula();
    void testStatisticalFormulaConfidence();
    void testStatisticalFormulaIntercept();
    void testFinacialODDLPRICEFormula();
    void testFinacialOddlyieldFormula();
    void testFinacialPriceDiscFormula();
    void testFinancialDBFormula();
    void testFinancialCouppcdFormula();
    void testFinancialCoupncdFormula();
    void testStatisticalFormulaLogInv();
    void testMathFormulaArcCot();
    void testMathFormulaCosh();
    void testStatisticalFormulaCritBinom();
    void testMathFormulaArcCotHyp();
    void testMathFormulaArcSin();
    void testMathFormulaArcSinHyp();
    void testMathFormulaArcTan();
    void testMathFormulaArcTanHyp();
    void testMathFormulaBitAnd();
    void testStatisticalFormulaForecast();
    void testStatisticalFormulaLogNormDist();
    void testStatisticalFormulaGammaDist();
    void testMathFormulaLN();
    void testMathFormulaRound();
    void testMathFormulaCot();
    void testMathFormulaCoth();
    void testFinacialNPER1Formula();
    void testStatisticalFormulaFDist();
    void testStatisticalFormulaVar();
    void testStatisticalFormulaChiDist();
    void testMathFormulaPower();
    void testMathFormulaOdd();
    void testStatisticalFormulaChiSqDist();
    void testStatisticalFormulaChiSqInv();
    void testStatisticalFormulaGammaInv();
    void testMathFormulaFloor();
    void testStatisticalFormulaFInv();
    void testStatisticalFormulaFTest();
    void testStatisticalFormulaB();
    void testStatisticalFormulaBetaDist();
    void testMathFormulaCscH();
    void testMathFormulaExp();
    void testMathFormulaLog10();
    void testStatisticalFormulaExpondist();
    void testMathAverageIfsFormula();
    void testMathCountIfsFormula();
    void testMathFormulaCombina();
    void testMathFormulaEven();
    void testMathFormulaLog();
    void testMathFormulaMod();
    void testMathFormulaTrunc();
    void testStatisticalFormulaSkew();
    void testMathFormulaArcTan2();
    void testMathFormulaBitOr();
    void testMathFormulaBitLshift();
    void testMathFormulaBitRshift();
    void testMathFormulaBitXor();
    void testStatisticalFormulaChiInv();
    void testStatisticalFormulaPoisson();
    void testMathFormulaSumSQ();
    void testStatisticalFormulaSkewp();
    void testMathFormulaSqrtPi();
    void testStatisticalFormulaBinomDist();
    void testStatisticalFormulaVarP();
    void testMathFormulaCeil();
    // void testMathFormulaKombin();
    void testStatisticalFormulaDevSq();
    void testStatisticalFormulaStDev();
    void testStatisticalFormulaSlope();
    void testStatisticalFormulaSTEYX();
    void testStatisticalFormulaZTest();
    void testMathFormulaPi();
    void testMathFormulaRandom();
    void testMathFormulaConvert();
    void testMathFormulaProduct();
    void testStatisticalFormulaHypGeomDist();
    void testArrayFormulaSumX2MY2();
    void testArrayFormulaSumX2PY2();
    void testStatisticalFormulaBetainv();
    void testStatisticalFormulaTTest();
    void testStatisticalFormulaTDist();
    void testStatisticalFormulaTInv();
    void testArrayFormulaSumXMY2();
    void testStatisticalFormulaStDevP();
    void testStatisticalFormulaCovar();
    void testLogicalFormulaAnd();
    void testLogicalFormulaOr();
    void testMathFormulaSumProduct();
    void testMathFormulaSumProduct2();
    void testStatisticalParallelCountBug();
    void testSpreadSheetFormulaVLookup();
    void testLogicalFormulaNot();
    void testLogicalFormulaXor();
    void testDatabaseFormulaDmax();
    void testDatabaseFormulaDmin();
    void testDatabaseFormulaDproduct();
    void testDatabaseFormulaDaverage();
    void testDatabaseFormulaDstdev();
    void testDatabaseFormulaDstdevp();
    void testDatabaseFormulaDsum();
    void testDatabaseFormulaDvar();
    void testDatabaseFormulaDvarp();
    void testMathFormulaAverageIf();
    void testDatabaseFormulaDcount();
    void testDatabaseFormulaDcountA();
    void testMathFormulaDegrees();
    void testMathFormulaRoundUp();
    void testMathFormulaRoundDown();
    void testMathFormulaInt();
    void testMathFormulaRadians();
    void testMathFormulaCountIf();
    void testMathFormulaIsEven();
    void testMathFormulaIsOdd();
    void testMathFormulaFact();
    void testStatisticalFormulaMina();
    void testStatisticalFormulaCountA();
    void testStatisticalFormulaMaxa();
    void testStatisticalFormulaAverageA();
    void testStatisticalFormulaVarA();
    void testStatisticalFormulaVarPA();
    void testStatisticalFormulaStDevA();
    void testStatisticalFormulaStDevPA();
    void testMathFormulaSEC();
    void testMathFormulaSECH();
    void testMathFormulaMROUND();
    void testMathFormulaSeriesSum();
    void testMathFormulaQuotient();
    void testMathFormulaSumIf();
    void testAddInFormulaBesseLJ();
    void testNegSub();
    void testStatisticalFormulaAvedev();
    void testMathFormulaAverageIf_Mix();
    void testStatisticalFormulaKurt1();
    void testStatisticalFormulaHarMean1();
    void testStatisticalFormulaVarA1();
    void testStatisticalFormulaVarPA1();
    void testStatisticalFormulaStDevA1();
    void testStatisticalFormulaStDevPA1();
    void testFinancialMDurationFormula1();

    CPPUNIT_TEST_SUITE(ScOpenCLTest);
    CPPUNIT_TEST(testSystematic);
    CPPUNIT_TEST(testSharedFormulaXLS);
    CPPUNIT_TEST(testFinacialFormula);
    CPPUNIT_TEST(testStatisticalFormulaFisher);
    CPPUNIT_TEST(testStatisticalFormulaFisherInv);
    CPPUNIT_TEST(testStatisticalFormulaGamma);
    CPPUNIT_TEST(testFinacialFvscheduleFormula);
// this test has intermittent failures on OSX
#if !defined MACOSX
    CPPUNIT_TEST(testFinacialIRRFormula);
#endif
    CPPUNIT_TEST(testFinacialMIRRFormula);
    CPPUNIT_TEST(testFinacialRateFormula);
    CPPUNIT_TEST(testCompilerHorizontal);
    CPPUNIT_TEST(testCompilerNested);
    CPPUNIT_TEST(testFinacialSLNFormula);
    CPPUNIT_TEST(testFinancialAccrintmFormula);
    CPPUNIT_TEST(testStatisticalFormulaGammaLn);
    CPPUNIT_TEST(testStatisticalFormulaGauss);
    CPPUNIT_TEST(testStatisticalFormulaGeoMean);
    CPPUNIT_TEST(testStatisticalFormulaHarMean);
    CPPUNIT_TEST(testFinancialCoupdaybsFormula);
    CPPUNIT_TEST(testFinacialDollardeFormula);
    CPPUNIT_TEST(testCompilerString);
    CPPUNIT_TEST(testCompilerInEq);
    CPPUNIT_TEST(testFinacialDollarfrFormula);
    CPPUNIT_TEST(testFinacialSYDFormula);
    CPPUNIT_TEST(testStatisticalFormulaCorrel);
    CPPUNIT_TEST(testFinancialCoupdaysFormula);
    CPPUNIT_TEST(testFinancialCoupdaysncFormula);
    CPPUNIT_TEST(testFinacialDISCFormula);
    CPPUNIT_TEST(testFinacialINTRATEFormula);
    CPPUNIT_TEST(testMathFormulaCos);
    CPPUNIT_TEST(testStatisticalFormulaNegbinomdist);
    CPPUNIT_TEST(testStatisticalFormulaRsq);
    CPPUNIT_TEST(testStatisticalFormulaPearson);
    CPPUNIT_TEST(testMathFormulaCsc);
    CPPUNIT_TEST(testFinacialPriceMatFormula);
    CPPUNIT_TEST(testFinacialXNPVFormula);
    CPPUNIT_TEST(testFinacialFormulaReceived);
    CPPUNIT_TEST(testFinancialFormulaCumipmt);
    CPPUNIT_TEST(testFinancialFormulaCumprinc);
    CPPUNIT_TEST(testFinacialRRIFormula);
    CPPUNIT_TEST(testFinacialEFFECT_ADDFormula);
    CPPUNIT_TEST(testFinacialNominalFormula);
    CPPUNIT_TEST(testFinacialTBILLEQFormula);
    CPPUNIT_TEST(testFinacialTBILLPRICEFormula);
    CPPUNIT_TEST(testFinacialTBILLYIELDFormula);
    CPPUNIT_TEST(testFinacialYIELDFormula);
    CPPUNIT_TEST(testFinacialYIELDDISCFormula);
    CPPUNIT_TEST(testFinacialYIELDMATFormula);
    CPPUNIT_TEST(testFinacialPPMTFormula);
    CPPUNIT_TEST(testFinacialPMTFormula);
    CPPUNIT_TEST(testFinancialISPMTFormula);
    CPPUNIT_TEST(testFinacialPriceFormula);
    CPPUNIT_TEST(testFinancialDurationFormula);
    CPPUNIT_TEST(testFinancialCoupnumFormula);
    CPPUNIT_TEST(testMathFormulaSinh);
    CPPUNIT_TEST(testMathFormulaAbs);
    CPPUNIT_TEST(testFinacialPVFormula);
    CPPUNIT_TEST(testMathFormulaSin);
    CPPUNIT_TEST(testMathFormulaTan);
    CPPUNIT_TEST(testMathFormulaTanH);
    CPPUNIT_TEST(testStatisticalFormulaStandard);
    CPPUNIT_TEST(testStatisticalFormulaWeibull);
    CPPUNIT_TEST(testStatisticalFormulaMedian);
    CPPUNIT_TEST(testFinancialDuration_ADDFormula);
    CPPUNIT_TEST(testFinancialAmordegrcFormula);
    CPPUNIT_TEST(testFinancialAmorlincFormula);
    CPPUNIT_TEST(testFinancialDDBFormula);
    CPPUNIT_TEST(testFinancialFVFormula);
    CPPUNIT_TEST(testFinancialMDurationFormula);
    CPPUNIT_TEST(testMathSumIfsFormula);
    CPPUNIT_TEST(testFinancialVDBFormula);
    CPPUNIT_TEST(testStatisticalFormulaKurt);
    CPPUNIT_TEST(testFinacialNPERFormula);
    CPPUNIT_TEST(testStatisticalFormulaNormdist);
    CPPUNIT_TEST(testMathFormulaArcCos);
    CPPUNIT_TEST(testMathFormulaSqrt);
    CPPUNIT_TEST(testMathFormulaArcCosHyp);
    CPPUNIT_TEST(testFinacialNPVFormula);
    CPPUNIT_TEST(testStatisticalFormulaNormsdist);
    CPPUNIT_TEST(testStatisticalFormulaNorminv);
    CPPUNIT_TEST(testStatisticalFormulaNormsinv);
    CPPUNIT_TEST(testStatisticalFormulaPermut);
    CPPUNIT_TEST(testStatisticalFormulaPermutation);
    CPPUNIT_TEST(testStatisticalFormulaPhi);
    CPPUNIT_TEST(testFinancialIPMTFormula);
    CPPUNIT_TEST(testStatisticalFormulaConfidence);
    CPPUNIT_TEST(testStatisticalFormulaIntercept);
    CPPUNIT_TEST(testFinacialODDLPRICEFormula);
    CPPUNIT_TEST(testFinacialOddlyieldFormula);
    CPPUNIT_TEST(testFinacialPriceDiscFormula);
    CPPUNIT_TEST(testFinancialDBFormula);
    CPPUNIT_TEST(testFinancialCouppcdFormula);
    CPPUNIT_TEST(testFinancialCoupncdFormula);
    CPPUNIT_TEST(testFinancialAccrintFormula);
    CPPUNIT_TEST(testStatisticalFormulaLogInv);
    CPPUNIT_TEST(testMathFormulaArcCot);
    CPPUNIT_TEST(testMathFormulaCosh);
    CPPUNIT_TEST(testStatisticalFormulaCritBinom);
    CPPUNIT_TEST(testMathFormulaArcCotHyp);
    CPPUNIT_TEST(testMathFormulaArcSin);
    CPPUNIT_TEST(testMathFormulaArcSinHyp);
    CPPUNIT_TEST(testMathFormulaArcTan);
    CPPUNIT_TEST(testMathFormulaArcTanHyp);
    CPPUNIT_TEST(testMathFormulaBitAnd);
    CPPUNIT_TEST(testStatisticalFormulaForecast);
    CPPUNIT_TEST(testStatisticalFormulaLogNormDist);
    CPPUNIT_TEST(testStatisticalFormulaGammaDist);
    CPPUNIT_TEST(testMathFormulaLN);
    CPPUNIT_TEST(testMathFormulaRound);
    CPPUNIT_TEST(testMathFormulaCot);
    CPPUNIT_TEST(testMathFormulaCoth);
    CPPUNIT_TEST(testFinacialNPER1Formula);
    CPPUNIT_TEST(testStatisticalFormulaFDist);
    CPPUNIT_TEST(testStatisticalFormulaVar);
    CPPUNIT_TEST(testStatisticalFormulaChiDist);
    CPPUNIT_TEST(testMathFormulaPower);
    CPPUNIT_TEST(testMathFormulaOdd);
    CPPUNIT_TEST(testStatisticalFormulaChiSqDist);
    CPPUNIT_TEST(testStatisticalFormulaChiSqInv);
    CPPUNIT_TEST(testStatisticalFormulaGammaInv);
    CPPUNIT_TEST(testMathFormulaFloor);
    CPPUNIT_TEST(testStatisticalFormulaFInv);
    CPPUNIT_TEST(testStatisticalFormulaFTest);
    CPPUNIT_TEST(testStatisticalFormulaB);
    CPPUNIT_TEST(testStatisticalFormulaBetaDist);
    CPPUNIT_TEST(testMathFormulaCscH);
    CPPUNIT_TEST(testMathFormulaExp);
    CPPUNIT_TEST(testMathFormulaLog10);
    CPPUNIT_TEST(testStatisticalFormulaExpondist);
    CPPUNIT_TEST(testMathAverageIfsFormula);
    CPPUNIT_TEST(testMathCountIfsFormula);
    CPPUNIT_TEST(testMathFormulaCombina);
    CPPUNIT_TEST(testMathFormulaEven);
    CPPUNIT_TEST(testMathFormulaLog);
    CPPUNIT_TEST(testMathFormulaMod);
    CPPUNIT_TEST(testMathFormulaTrunc);
    CPPUNIT_TEST(testStatisticalFormulaSkew);
    CPPUNIT_TEST(testMathFormulaArcTan2);
    CPPUNIT_TEST(testMathFormulaBitOr);
    CPPUNIT_TEST(testMathFormulaBitLshift);
    CPPUNIT_TEST(testMathFormulaBitRshift);
    CPPUNIT_TEST(testMathFormulaBitXor);
    CPPUNIT_TEST(testStatisticalFormulaChiInv);
    CPPUNIT_TEST(testStatisticalFormulaPoisson);
    CPPUNIT_TEST(testMathFormulaSumSQ);
    CPPUNIT_TEST(testStatisticalFormulaSkewp);
    CPPUNIT_TEST(testMathFormulaSqrtPi);
    CPPUNIT_TEST(testStatisticalFormulaBinomDist);
    CPPUNIT_TEST(testStatisticalFormulaVarP);
    CPPUNIT_TEST(testMathFormulaCeil);
    // This test fails MacOS 10.8. Disabled temporarily
    // CPPUNIT_TEST(testMathFormulaKombin);
    CPPUNIT_TEST(testStatisticalFormulaDevSq);
    CPPUNIT_TEST(testStatisticalFormulaStDev);
    CPPUNIT_TEST(testStatisticalFormulaSlope);
    CPPUNIT_TEST(testStatisticalFormulaSTEYX);
    CPPUNIT_TEST(testStatisticalFormulaZTest);
    CPPUNIT_TEST(testMathFormulaPi);
    CPPUNIT_TEST(testMathFormulaRandom);
    CPPUNIT_TEST(testMathFormulaConvert);
    CPPUNIT_TEST(testMathFormulaProduct);
    CPPUNIT_TEST(testStatisticalFormulaHypGeomDist);
    CPPUNIT_TEST(testArrayFormulaSumX2MY2);
    CPPUNIT_TEST(testArrayFormulaSumX2PY2);
    CPPUNIT_TEST(testStatisticalFormulaBetainv);
    CPPUNIT_TEST(testStatisticalFormulaTTest);
    CPPUNIT_TEST(testStatisticalFormulaTDist);
    CPPUNIT_TEST(testStatisticalFormulaTInv);
    CPPUNIT_TEST(testArrayFormulaSumXMY2);
    CPPUNIT_TEST(testStatisticalFormulaStDevP);
    CPPUNIT_TEST(testStatisticalFormulaCovar);
    CPPUNIT_TEST(testLogicalFormulaAnd);
    CPPUNIT_TEST(testMathFormulaSumProduct);
    CPPUNIT_TEST(testMathFormulaSumProduct2);
    CPPUNIT_TEST(testStatisticalParallelCountBug);
    CPPUNIT_TEST(testSpreadSheetFormulaVLookup);
    CPPUNIT_TEST(testLogicalFormulaOr);
    CPPUNIT_TEST(testLogicalFormulaNot);
    CPPUNIT_TEST(testLogicalFormulaXor);
    CPPUNIT_TEST(testDatabaseFormulaDmax);
    CPPUNIT_TEST(testDatabaseFormulaDmin);
    CPPUNIT_TEST(testDatabaseFormulaDproduct);
    CPPUNIT_TEST(testDatabaseFormulaDaverage);
    CPPUNIT_TEST(testDatabaseFormulaDstdev);
    CPPUNIT_TEST(testDatabaseFormulaDstdevp);
    CPPUNIT_TEST(testDatabaseFormulaDsum);
    CPPUNIT_TEST(testDatabaseFormulaDvar);
    CPPUNIT_TEST(testDatabaseFormulaDvarp);
    CPPUNIT_TEST(testMathFormulaAverageIf);
    CPPUNIT_TEST(testDatabaseFormulaDcount);
    CPPUNIT_TEST(testDatabaseFormulaDcountA);
    CPPUNIT_TEST(testMathFormulaDegrees);
    CPPUNIT_TEST(testMathFormulaRoundUp);
    CPPUNIT_TEST(testMathFormulaRoundDown);
    CPPUNIT_TEST(testMathFormulaInt);
    CPPUNIT_TEST(testMathFormulaRadians);
    CPPUNIT_TEST(testMathFormulaCountIf);
    CPPUNIT_TEST(testMathFormulaIsEven);
    CPPUNIT_TEST(testMathFormulaIsOdd);
    CPPUNIT_TEST(testMathFormulaFact);
    CPPUNIT_TEST(testStatisticalFormulaMaxa);
    CPPUNIT_TEST(testStatisticalFormulaMina);
    CPPUNIT_TEST(testStatisticalFormulaCountA);
    CPPUNIT_TEST(testStatisticalFormulaAverageA);
    CPPUNIT_TEST(testStatisticalFormulaVarA);
    CPPUNIT_TEST(testStatisticalFormulaVarPA);
    CPPUNIT_TEST(testStatisticalFormulaStDevA);
    CPPUNIT_TEST(testStatisticalFormulaStDevPA);
    CPPUNIT_TEST(testMathFormulaSEC);
    CPPUNIT_TEST(testMathFormulaSECH);
    CPPUNIT_TEST(testMathFormulaMROUND);
    CPPUNIT_TEST(testMathFormulaQuotient);
    CPPUNIT_TEST(testMathFormulaSeriesSum);
    CPPUNIT_TEST(testMathFormulaSumIf);
    CPPUNIT_TEST(testAddInFormulaBesseLJ);
    CPPUNIT_TEST(testNegSub);
    CPPUNIT_TEST(testStatisticalFormulaAvedev);
    CPPUNIT_TEST(testMathFormulaAverageIf_Mix);
    CPPUNIT_TEST(testStatisticalFormulaKurt1);
    CPPUNIT_TEST(testStatisticalFormulaHarMean1);
    CPPUNIT_TEST(testStatisticalFormulaVarA1);
    CPPUNIT_TEST(testStatisticalFormulaVarPA1);
    CPPUNIT_TEST(testStatisticalFormulaStDevA1);
    CPPUNIT_TEST(testStatisticalFormulaStDevPA1);
    CPPUNIT_TEST(testFinancialMDurationFormula1);
    CPPUNIT_TEST_SUITE_END();

private:
    uno::Reference<uno::XInterface> m_xCalcComponent;

    // Test env variables and methods
    ScDocShellRef xDocSh;
    ScDocShellRef xDocShRes;
    bool initTestEnv(std::u16string_view fileName, sal_Int32 nFormat,
              bool bReadWrite);
};

bool ScOpenCLTest::initTestEnv(std::u16string_view fileName, sal_Int32 nFormat,
    bool bReadWrite)
{
    if(!detectOpenCLDevice())
        return false;

    xDocSh = loadDoc(fileName, nFormat, bReadWrite);
    enableOpenCL();

    xDocShRes = loadDoc(fileName, nFormat, bReadWrite);

    return true;
}

bool ScOpenCLTest::detectOpenCLDevice()
{
    sc::FormulaGroupInterpreter::enableOpenCL_UnitTestsOnly();
    return sc::FormulaGroupInterpreter::switchOpenCLDevice(u"",true);
}

void ScOpenCLTest::enableOpenCL()
{
    sc::FormulaGroupInterpreter::enableOpenCL_UnitTestsOnly();
}

void ScOpenCLTest::testCompilerHorizontal()
{
    if(!initTestEnv(u"opencl/compiler/horizontal.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i < 5; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(12, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(12, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        fLibre = rDoc.GetValue(ScAddress(13, i, 0));
        fExcel = rDocRes.GetValue(ScAddress(13, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        fLibre = rDoc.GetValue(ScAddress(14, i, 0));
        fExcel = rDocRes.GetValue(ScAddress(14, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testCompilerNested()
{
    if(!initTestEnv(u"opencl/compiler/nested.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i < 5; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testCompilerString()
{
    if(!initTestEnv(u"opencl/compiler/string.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i < 5; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));

        fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testCompilerInEq()
{
    if(!initTestEnv(u"opencl/compiler/ineq.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i < 7; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

#if 0
void ScOpenCLTest::testSharedFormulaXLSStockHistory()
{
    if(!initTestEnv("stock-history.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    xDocSh->DoHardRecalc();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 33; i < 44; ++i)
    {   // Cell H34:H44 in S&P 500 (tab 1)
        double fLibre = rDoc.GetValue(ScAddress(7, i, 1));
        double fExcel = rDocRes.GetValue(ScAddress(7, i, 1));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, 0.0001*fExcel);
    }

    for (SCROW i = 33; i < 44; ++i)
    {   // Cell J34:J44 in S&P 500 (tab 1)
        double fLibre = rDoc.GetValue(ScAddress(9, i, 1));
        double fExcel = rDocRes.GetValue(ScAddress(9, i, 1));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, 0.0001*fExcel);
    }
}

void ScOpenCLTest::testSharedFormulaXLSGroundWater()
{
    if(!initTestEnv("ground-water-daily.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    xDocSh->DoHardRecalc();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 5; i <= 77; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(11,i,1));
        double fExcel = rDocRes.GetValue(ScAddress(11,i,1));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }


}
#endif

void ScOpenCLTest::testSystematic()
{
    if(!initTestEnv(u"systematic.", FORMAT_XLS, false))
        return;

    ScDocument& rDoc = xDocSh->GetDocument();
    rDoc.CalcAll();

    int nAVertBegin(0), nAVertEnd(0), nBVertBegin(0), nBVertEnd(0);
    int nAHorEnd(0), nBHorEnd(0);

    int nRow, nCol;
    for (nRow = 0; nRow < 1000; ++nRow)
    {
        if (rDoc.GetString(ScAddress(0, nRow, 0)) == "a")
        {
            nAVertBegin = nRow + 1;

            for (nCol = 0; nCol < 1000; ++nCol)
            {
                if (rDoc.GetString(ScAddress(nCol, nRow, 0)) != "a")
                {
                    nAHorEnd = nCol;
                    break;
                }
            }
            break;
        }
    }
    for (; nRow < 1000; ++nRow)
    {
        if (rDoc.GetString(ScAddress(0, nRow, 0)) != "a")
        {
            nAVertEnd = nRow;
            break;
        }
    }

    for (; nRow < 1000; ++nRow)
    {
        if (rDoc.GetString(ScAddress(0, nRow, 0)) == "b")
        {
            nBVertBegin = nRow + 1;

            for (nCol = 0; nCol < 1000; ++nCol)
            {
                if (rDoc.GetString(ScAddress(nCol, nRow, 0)) != "b")
                {
                    nBHorEnd = nCol;
                    break;
                }
            }
            break;
        }
    }
    for (; nRow < 1000; ++nRow)
    {
        if (rDoc.GetString(ScAddress(0, nRow, 0)) != "b")
        {
            nBVertEnd = nRow;
            break;
        }
    }

    CPPUNIT_ASSERT(nAVertBegin != 0);
    CPPUNIT_ASSERT(nBVertBegin != 0);
    CPPUNIT_ASSERT(nAVertEnd > nAVertBegin + 100);
    CPPUNIT_ASSERT(nBVertEnd > nBVertBegin + 100);
    CPPUNIT_ASSERT_EQUAL(nAVertEnd-nAVertBegin, nBVertEnd-nBVertBegin);
    CPPUNIT_ASSERT(nAHorEnd > 10);
    CPPUNIT_ASSERT(nBHorEnd > 10);
    CPPUNIT_ASSERT_EQUAL(nAHorEnd, nBHorEnd);

    for (SCROW i = nAVertBegin; i < nAVertEnd; ++i)
    {
        for (int j = 1; j < nAHorEnd; ++j)
        {
            double fLibre = rDoc.GetValue(ScAddress(j, i, 0));
            double fExcel = rDoc.GetValue(ScAddress(j, nBVertBegin + (i - nAVertBegin), 0));

            const OString sFailedMessage =
                OStringChar(static_cast<char>('A'+j)) +
                OString::number(i+1) +
                "!=" +
                OStringChar(static_cast<char>('A'+j)) +
                OString::number(nBVertBegin+(i-nAVertBegin)+1);
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(sFailedMessage.getStr(), fExcel, fLibre, 1e-10);
        }
    }
}


void ScOpenCLTest::testSharedFormulaXLS()
{
    if(!initTestEnv(u"sum_ex.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i < 5; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    for (SCROW i = 6; i < 14; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    for (SCROW i = 15; i < 18; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    for (SCROW i = 19; i < 22; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    for (SCROW i = 23; i < 25; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        //double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        // There seems to be a bug in LibreOffice beta
        ASSERT_DOUBLES_EQUAL(/*fExcel*/ 60.0, fLibre);
    }

    for (SCROW i = 25; i < 27; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    for (SCROW i = 28; i < 35; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    // workaround for a Calc beta bug
    ASSERT_DOUBLES_EQUAL(25.0, rDoc.GetValue(ScAddress(2, 35, 0)));
    ASSERT_DOUBLES_EQUAL(24.0, rDoc.GetValue(ScAddress(2, 36, 0)));

    for (SCROW i = 38; i < 43; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    for (SCROW i = 5; i < 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 1));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 1));
        ASSERT_DOUBLES_EQUAL(fExcel, fLibre);
    }

    for (SCROW i = 5; i < 10; ++i)
    {
        for (SCCOL j = 6; j < 11; ++j)
        {
            double fLibre = rDoc.GetValue(ScAddress(j, i, 1));
            double fExcel = rDocRes.GetValue(ScAddress(j, i, 1));
            CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre,
                fabs(fExcel*0.0001));
        }
    }
}

void ScOpenCLTest::testMathFormulaCos()
{
    if(!initTestEnv(u"opencl/math/cos.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSinh()
{
    if(!initTestEnv(u"opencl/math/sinh.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    xDocSh->DoHardRecalc();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaPi()
{
    if(!initTestEnv(u"opencl/math/pi.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(0,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(0,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaRandom()
{
    if(!initTestEnv(u"opencl/math/random.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        rDoc.GetValue(ScAddress(0,i,0)); // LO
        rDocRes.GetValue(ScAddress(0,i,0)); // Excel
        //because the random numbers will always change,so give the test "true"
        CPPUNIT_ASSERT(true);
    }
}
void ScOpenCLTest::testFinacialFormula()
{
    if(!initTestEnv(u"opencl/financial/general.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,1));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,1));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,2));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,2));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,3));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,3));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,4));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,4));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,5));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,5));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 0; i < 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,6));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,6));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,7));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,7));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,8));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,8));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,9));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,9));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,10));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,10));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7,i,11));
        double fExcel = rDocRes.GetValue(ScAddress(7,i,11));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,12));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,12));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 0; i <= 12; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,13));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,13));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,14));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,14));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,15));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,15));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,16));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,16));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 1; i <= 5; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,17));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,17));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 0; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,18));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,18));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 0; i <= 18; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,19));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,19));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaCorrel()
{
    if(!initTestEnv(u"opencl/statistical/Correl.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testStatisticalFormulaFisher()
{
    if(!initTestEnv(u"opencl/statistical/Fisher.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaFisherInv()
{
    if(!initTestEnv(u"opencl/statistical/FisherInv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaGamma()
{
    if(!initTestEnv(u"opencl/statistical/Gamma.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialFvscheduleFormula()
{
    if(!initTestEnv(u"opencl/financial/Fvschedule.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaAbs()
{
    if(!initTestEnv(u"opencl/math/Abs.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ABS Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialSYDFormula()
{
    if(!initTestEnv(u"opencl/financial/SYD.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

// this test has intermittent failures on OSX
#if !defined MACOSX
void ScOpenCLTest::testFinacialIRRFormula()
{
    if(!initTestEnv(u"opencl/financial/IRR.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
#endif

void ScOpenCLTest::testStatisticalFormulaGammaLn()
{
    if(!initTestEnv(u"opencl/statistical/GammaLn.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaGauss()
{
    if(!initTestEnv(u"opencl/statistical/Gauss.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaGeoMean()
{
    if(!initTestEnv(u"opencl/statistical/GeoMean.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaHarMean()
{
    if(!initTestEnv(u"opencl/statistical/HarMean.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialSLNFormula()
{
    if(!initTestEnv(u"opencl/financial/SLN.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialMIRRFormula()
{
    if(!initTestEnv(u"opencl/financial/MIRR.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialCoupdaybsFormula()
{
    if(!initTestEnv(u"opencl/financial/Coupdaybs.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <=10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialDollardeFormula()
{
    if(!initTestEnv(u"opencl/financial/Dollarde.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialCoupdaysFormula()
{
    if(!initTestEnv(u"opencl/financial/Coupdays.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <=10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

}

void ScOpenCLTest::testFinancialCoupdaysncFormula()
{
    if(!initTestEnv(u"opencl/financial/Coupdaysnc.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <=10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialRateFormula()
{
    if(!initTestEnv(u"opencl/financial/RATE.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <= 5; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialAccrintmFormula()
{
    if(!initTestEnv(u"opencl/financial/Accrintm.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialCoupnumFormula()
{
    if(!initTestEnv(u"opencl/financial/Coupnum.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaNegbinomdist()
{
    if(!initTestEnv(u"opencl/statistical/Negbinomdist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSin()
{
    if(!initTestEnv(u"opencl/math/sin.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSumSQ()
{
    if(!initTestEnv(u"opencl/math/sumsq.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i < 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaTan()
{
    if(!initTestEnv(u"opencl/math/tan.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaTanH()
{
    if(!initTestEnv(u"opencl/math/tanh.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSqrt()
{
    if(!initTestEnv(u"opencl/math/sqrt.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialPriceFormula()
{
    if(!initTestEnv(u"opencl/financial/Price.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(7, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialDollarfrFormula()
{
    if(!initTestEnv(u"opencl/financial/Dollarfr.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialPriceDiscFormula()
{
    if(!initTestEnv(u"opencl/financial/PriceDisc.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialODDLPRICEFormula()
{
    if(!initTestEnv(u"opencl/financial/Oddlprice.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(8, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(8, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinacialOddlyieldFormula()
{
    if(!initTestEnv(u"opencl/financial/Oddlyield.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(8, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(8, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialDISCFormula()
{
    if(!initTestEnv(u"opencl/financial/DISC.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinacialPVFormula()
{
    if(!initTestEnv(u"opencl/financial/PV.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialINTRATEFormula()
{
    if(!initTestEnv(u"opencl/financial/INTRATE.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaStandard()
{
    if(!initTestEnv(u"opencl/statistical/Standard.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaWeibull()
{
    if(!initTestEnv(u"opencl/statistical/Weibull.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre,
            fExcel == 0?1e-4:fabs(1e-4*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaVar()
{
    if(!initTestEnv(u"opencl/statistical/Var.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaSkew()
{
    if(!initTestEnv(u"opencl/statistical/Skew.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaSkewp()
{
    if(!initTestEnv(u"opencl/statistical/Skewp.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaPearson()
{
    if(!initTestEnv(u"opencl/statistical/Pearson.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaRsq()
{
    if(!initTestEnv(u"opencl/statistical/Rsq.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaTrunc()
{
    if(!initTestEnv(u"opencl/math/trunc.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCosh()
{
    if(!initTestEnv(u"opencl/math/cosh.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testStatisticalFormulaCovar()
{
    if(!initTestEnv(u"opencl/statistical/Covar.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 16; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaKurt()
{
    if(!initTestEnv(u"opencl/statistical/Kurt.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCot()
{
    if(!initTestEnv(u"opencl/math/cot.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaDevSq()
{
    if(!initTestEnv(u"opencl/statistical/DevSq.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 11; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCsc()
{
    if(!initTestEnv(u"opencl/math/csc.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCoth()
{
    if(!initTestEnv(u"opencl/math/coth.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialXNPVFormula()
{
    if(!initTestEnv(u"opencl/financial/XNPV.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }

    for (SCROW i = 16; i <= 26; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaIntercept()
{
    if(!initTestEnv(u"opencl/statistical/Intercept.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialAmordegrcFormula()
{
    if(!initTestEnv(u"opencl/financial/Amordegrc.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(7, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialISPMTFormula()
{
    if(!initTestEnv(u"opencl/financial/ISPMT.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaMedian()
{
    if(!initTestEnv(u"opencl/statistical/Median.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaNormdist()
{
    if(!initTestEnv(u"opencl/statistical/Normdist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaNormsdist()
{
    if(!initTestEnv(u"opencl/statistical/Normsdist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaPermut()
{
    if(!initTestEnv(u"opencl/statistical/Permut.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaPermutation()
{
    if(!initTestEnv(u"opencl/statistical/Permutation.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaPhi()
{
    if(!initTestEnv(u"opencl/statistical/Phi.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCscH()
{
    if(!initTestEnv(u"opencl/math/csch.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaLogInv()
{
    if(!initTestEnv(u"opencl/statistical/LogInv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialNPERFormula()
{
    if(!initTestEnv(u"opencl/financial/NPER.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaForecast()
{
    if(!initTestEnv(u"opencl/statistical/Forecast.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialAmorlincFormula()
{
    if(!initTestEnv(u"opencl/financial/Amorlinc.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(7, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialDDBFormula()
{
    if(!initTestEnv(u"opencl/financial/ddb.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialPriceMatFormula()
{
    if(!initTestEnv(u"opencl/financial/PriceMat.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialFormulaReceived()
{
    if(!initTestEnv(u"opencl/financial/Received.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i < 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinancialFormulaCumipmt()
{
    if(!initTestEnv(u"opencl/financial/Cumipmt.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinancialFormulaCumprinc()
{
    if(!initTestEnv(u"opencl/financial/Cumprinc.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialRRIFormula()
{
    if(!initTestEnv(u"opencl/financial/RRI.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialEFFECT_ADDFormula()
{
    if(!initTestEnv(u"opencl/financial/EFFECT_ADD.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialNominalFormula()
{
    if(!initTestEnv(u"opencl/financial/Nominal.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();


    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialTBILLEQFormula()
{
    if(!initTestEnv(u"opencl/financial/TBILLEQ.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialTBILLPRICEFormula()
{
    if(!initTestEnv(u"opencl/financial/TBILLPRICE.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialTBILLYIELDFormula()
{
    if(!initTestEnv(u"opencl/financial/TBILLYIELD.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest::testFinacialYIELDFormula()
{
    if(!initTestEnv(u"opencl/financial/YIELD.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(7, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialYIELDDISCFormula()
{
    if(!initTestEnv(u"opencl/financial/YIELDDISC.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinacialYIELDMATFormula()
{
    if(!initTestEnv(u"opencl/financial/YIELDMAT.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
void ScOpenCLTest:: testFinacialPMTFormula()
{
    if(!initTestEnv(u"opencl/financial/PMT.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialDurationFormula()
{
    if(!initTestEnv(u"opencl/financial/Duration.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaLogNormDist()
{
    if(!initTestEnv(u"opencl/statistical/LogNormDist.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcCos()
{
    if(!initTestEnv(u"opencl/math/ArcCos.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ACos Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaPower()
{
    if(!initTestEnv(u"opencl/math/power.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinacialPPMTFormula()
{
    if(!initTestEnv(u"opencl/financial/PPMT.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinacialNPVFormula()
{
    if(!initTestEnv(u"opencl/financial/NPV.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialDuration_ADDFormula()
{
    if(!initTestEnv(u"opencl/financial/Duration_ADD.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaNorminv()
{
    if(!initTestEnv(u"opencl/statistical/Norminv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaNormsinv()
{
    if(!initTestEnv(u"opencl/statistical/Normsinv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcCosHyp()
{
    if(!initTestEnv(u"opencl/math/ArcCosHyp.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ACosH Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialMDurationFormula()
{
    if(!initTestEnv(u"opencl/financial/MDuration.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcCot()
{
    if(!initTestEnv(u"opencl/math/ArcCot.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ACot Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialFVFormula()
{
    if(!initTestEnv(u"opencl/financial/FV.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialDBFormula()
{
    if(!initTestEnv(u"opencl/financial/db.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialCouppcdFormula()
{
    if(!initTestEnv(u"opencl/financial/Couppcd.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathSumIfsFormula()
{
    if(!initTestEnv(u"opencl/math/sumifs.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    xDocSh->DoHardRecalc();

    for (SCROW i = 2; i <= 11; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 2; i <= 11; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 2; i <= 11; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(7,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 2; i <= 11; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(8,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(8,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcCotHyp()
{
    if(!initTestEnv(u"opencl/math/ArcCotHyp.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ACotH Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcSin()
{
    if(!initTestEnv(u"opencl/math/ArcSin.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ACotH Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialVDBFormula()
{
    if(!initTestEnv(u"opencl/financial/VDB.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(7, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 15; i <= 26; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 30; i <= 41; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialIPMTFormula()
{
    if(!initTestEnv(u"opencl/financial/IPMT.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaChiSqDist()
{
    if(!initTestEnv(u"opencl/statistical/CHISQDIST.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaConfidence()
{
    if(!initTestEnv(u"opencl/statistical/Confidence.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaFDist()
{
    if(!initTestEnv(u"opencl/statistical/Fdist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialCoupncdFormula()
{
    if(!initTestEnv(u"opencl/financial/Coupncd.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testFinancialAccrintFormula()
{
    if(!initTestEnv(u"opencl/financial/Accrint.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(7, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(7, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaCritBinom()
{
    if(!initTestEnv(u"opencl/statistical/CritBinom.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcSinHyp()
{
    if(!initTestEnv(u"opencl/math/ArcSinHyp.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ASinH Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcTan()
{
    if(!initTestEnv(u"opencl/math/ArcTan.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ATan Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcTanHyp()
{
    if(!initTestEnv(u"opencl/math/ArcTanHyp.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ATanH Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinacialNPER1Formula()
{
    if(!initTestEnv(u"opencl/financial/NPER1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 6; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(5, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaArcTan2()
{
    if(!initTestEnv(u"opencl/math/ArcTan2.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify ATan2 Function
    for (SCROW i = 1; i <= 17; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, 0.000001);
    }
}

void ScOpenCLTest::testStatisticalFormulaChiSqInv()
{
    if(!initTestEnv(u"opencl/statistical/CHISQINV.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaBitAnd()
{
    if(!initTestEnv(u"opencl/math/BitAnd.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify BitAnd Function
    for (SCROW i = 1; i <= 1000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaPoisson()
{
    if(!initTestEnv(u"opencl/statistical/Poisson.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaExpondist()
{
    if(!initTestEnv(u"opencl/statistical/Expondist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaBitOr()
{
    if(!initTestEnv(u"opencl/math/BitOr.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify BitOr Function
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaOdd()
{
    if(!initTestEnv(u"opencl/math/odd.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaLN()
{
    if(!initTestEnv(u"opencl/math/LN.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaMod()
{
    if(!initTestEnv(u"opencl/math/mod.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        if(fExcel == 0.0f)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, 1e-10);
        else
            CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaRound()
{
    if(!initTestEnv(u"opencl/math/ROUND.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 15; i <= 25; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaGammaDist()
{
    if(!initTestEnv(u"opencl/statistical/GammaDist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaGammaInv()
{
    if(!initTestEnv(u"opencl/statistical/GammaInv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        fLibre = rDoc.GetValue(ScAddress(4,i,0));
        fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaFInv()
{
    if(!initTestEnv(u"opencl/statistical/FInv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        fLibre = rDoc.GetValue(ScAddress(4,i,0));
        fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaFTest()
{
    if(!initTestEnv(u"opencl/statistical/FTest.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaB()
{
    if(!initTestEnv(u"opencl/statistical/B.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        fLibre = rDoc.GetValue(ScAddress(5,i,0));
        fExcel = rDocRes.GetValue(ScAddress(5,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaBetaDist()
{
    if(!initTestEnv(u"opencl/statistical/BetaDist.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        fLibre = rDoc.GetValue(ScAddress(7,i,0));
        fExcel = rDocRes.GetValue(ScAddress(7,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaEven()
{
    if(!initTestEnv(u"opencl/math/even.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaExp()
{
    if(!initTestEnv(u"opencl/math/exp.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaChiDist()
{
    if(!initTestEnv(u"opencl/statistical/ChiDist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaBitLshift()
{
    if(!initTestEnv(u"opencl/math/BitLshift.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify BitLshift Function
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaBitRshift()
{
    if(!initTestEnv(u"opencl/math/BitRshift.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify BitRshift Function
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaFloor()
{
    if(!initTestEnv(u"opencl/math/floor.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaLog()
{
    if(!initTestEnv(u"opencl/math/log.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 47; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testSpreadSheetFormulaVLookup()
{
    if(!initTestEnv(u"opencl/spreadsheet/VLookup.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 40; i <= 50; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaChiInv()
{
    if(!initTestEnv(u"opencl/statistical/ChiInv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaConvert()
{
    if(!initTestEnv(u"opencl/math/convert.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 3; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathCountIfsFormula()
{
    if(!initTestEnv(u"opencl/math/countifs.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    xDocSh->DoHardRecalc();

    for (SCROW i = 1; i < 10; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(4, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaBitXor()
{
    if(!initTestEnv(u"opencl/math/BitXor.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify BitXor Function
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathAverageIfsFormula()
{
    if(!initTestEnv(u"opencl/math/averageifs.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();

    xDocSh->DoHardRecalc();

    for (SCROW i = 1; i <= 11; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaLog10()
{
    if(!initTestEnv(u"opencl/math/log10.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCombina()
{
    if(!initTestEnv(u"opencl/math/combina.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 47; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCeil()
{
    if(!initTestEnv(u"opencl/math/Ceil.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify Ceiling Function
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSqrtPi()
{
    if(!initTestEnv(u"opencl/math/sqrtpi.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i < 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaVarP()
{
    if(!initTestEnv(u"opencl/statistical/VarP.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaStDev()
{
    if(!initTestEnv(u"opencl/statistical/StDev.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaStDevP()
{
    if(!initTestEnv(u"opencl/statistical/StDevP.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaSlope()
{
    if(!initTestEnv(u"opencl/statistical/Slope.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaSTEYX()
{
    if(!initTestEnv(u"opencl/statistical/STEYX.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaZTest()
{
    if(!initTestEnv(u"opencl/statistical/ZTest.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaTTest()
{
    if(!initTestEnv(u"opencl/statistical/TTest.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaTDist()
{
    if(!initTestEnv(u"opencl/statistical/TDist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaTInv()
{
    if(!initTestEnv(u"opencl/statistical/TInv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaBinomDist()
{
    if(!initTestEnv(u"opencl/statistical/BinomDist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaProduct()
{
    if(!initTestEnv(u"opencl/math/product.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 3; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

#if 0 //Disabled temporarily
void ScOpenCLTest::testMathFormulaKombin()
{
    if(!initTestEnv("opencl/math/Kombin.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify Combin Function
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}
#endif

void ScOpenCLTest:: testArrayFormulaSumX2MY2()
{
    if(!initTestEnv(u"opencl/array/SUMX2MY2.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 20; i <= 26; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaHypGeomDist()
{
    if(!initTestEnv(u"opencl/statistical/HypGeomDist.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testArrayFormulaSumX2PY2()
{
    if(!initTestEnv(u"opencl/array/SUMX2PY2.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 20; i <= 26; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaBetainv()
{
    if(!initTestEnv(u"opencl/statistical/Betainv.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(5,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(5,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaMina()
{
    if(!initTestEnv(u"opencl/statistical/Mina.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testArrayFormulaSumXMY2()
{
    if(!initTestEnv(u"opencl/array/SUMXMY2.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 20; i <= 26; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaCountA()
{
    if(!initTestEnv(u"opencl/statistical/counta.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaMaxa()
{
    if(!initTestEnv(u"opencl/statistical/Maxa.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSumProduct()
{
    if(!initTestEnv(u"opencl/math/sumproduct_mixSliding.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        if ( i == 1 )
            CPPUNIT_ASSERT_DOUBLES_EQUAL(82,  fLibre, fabs(0.0001*fExcel));
        else if ( i == 2 )
            CPPUNIT_ASSERT_DOUBLES_EQUAL(113, fLibre, fabs(0.0001*fExcel));
        else if ( i == 4 )
            CPPUNIT_ASSERT_DOUBLES_EQUAL(175, fLibre, fabs(0.0001*fExcel));
        else if ( i == 5 )
            CPPUNIT_ASSERT_DOUBLES_EQUAL(206, fLibre, fabs(0.0001*fExcel));
        else if ( i == 6 )
            CPPUNIT_ASSERT_DOUBLES_EQUAL(237, fLibre, fabs(0.0001*fExcel));
        else if ( i == 7 )
            CPPUNIT_ASSERT_DOUBLES_EQUAL(268, fLibre, fabs(0.0001*fExcel));
        else
            CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaAverageIf()
{
    if(!initTestEnv(u"opencl/math/averageif.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 2; i <= 21; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(6,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaAverageA()
{
    if(!initTestEnv(u"opencl/statistical/AverageA.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testLogicalFormulaAnd()
{
    if(!initTestEnv(u"opencl/logical/and.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(3, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaVarA()
{
    if(!initTestEnv(u"opencl/statistical/VarA.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaVarPA()
{
    if(!initTestEnv(u"opencl/statistical/VarPA.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();
    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaStDevA()
{
    if(!initTestEnv(u"opencl/statistical/StDevA.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaStDevPA()
{
    if(!initTestEnv(u"opencl/statistical/StDevPA.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testFinancialMDurationFormula1()
{
    if(!initTestEnv(u"opencl/financial/MDuration1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(6, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(6, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel,fLibre,fabs(0.00000000001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSumProduct2()
{
    if(!initTestEnv(u"opencl/math/sumproductTest.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 2; i <= 12; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(4,i,1));
        double fExcel = rDocRes.GetValue(ScAddress(4,i,1));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel,  fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testStatisticalParallelCountBug()
{
    if(!initTestEnv(u"opencl/statistical/parallel_count_bug_243.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i < 13; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testLogicalFormulaOr()
{
    if(!initTestEnv(u"opencl/logical/or.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i < 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(2, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testLogicalFormulaNot()
{
    if(!initTestEnv(u"opencl/logical/not.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i < 3000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(1, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest:: testLogicalFormulaXor()
{
    if(!initTestEnv(u"opencl/logical/xor.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i < 3000; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1, i, 0));
        double fExcel = rDocRes.GetValue(ScAddress(1, i, 0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testDatabaseFormulaDcount()
{
    if(!initTestEnv(u"opencl/database/dcount.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDcountA()
{
    if(!initTestEnv(u"opencl/database/dcountA.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDmax()
{
    if(!initTestEnv(u"opencl/database/dmax.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDmin()
{
    if(!initTestEnv(u"opencl/database/dmin.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDproduct()
{
    if(!initTestEnv(u"opencl/database/dproduct.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDaverage()
{
    if(!initTestEnv(u"opencl/database/daverage.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        //CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDstdev()
{
    if(!initTestEnv(u"opencl/database/dstdev.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        //CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDstdevp()
{
    if(!initTestEnv(u"opencl/database/dstdevp.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        //CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDsum()
{
    if(!initTestEnv(u"opencl/database/dsum.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        //CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDvar()
{
    if(!initTestEnv(u"opencl/database/dvar.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        //CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testDatabaseFormulaDvarp()
{
    if(!initTestEnv(u"opencl/database/dvarp.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 32; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(9,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(10,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.00000000001));
    }
}

void ScOpenCLTest::testMathFormulaRoundUp()
{
    if(!initTestEnv(u"opencl/math/roundup.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaRoundDown()
{
    if(!initTestEnv(u"opencl/math/rounddown.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaInt()
{
    if(!initTestEnv(u"opencl/math/int.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaRadians()
{
    if(!initTestEnv(u"opencl/math/radians.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaDegrees()
{
    if(!initTestEnv(u"opencl/math/degrees.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 200; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaIsEven()
{
    if(!initTestEnv(u"opencl/math/iseven.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaCountIf()
{
    if(!initTestEnv(u"opencl/math/countif.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 26; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaIsOdd()
{
    if(!initTestEnv(u"opencl/math/isodd.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaFact()
{
    if(!initTestEnv(u"opencl/math/fact.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 18; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSEC()
{
    if(!initTestEnv(u"opencl/math/sec.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSECH()
{
    if(!initTestEnv(u"opencl/math/sech.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaMROUND()
{
    if(!initTestEnv(u"opencl/math/MROUND.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 13; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaQuotient()
{
    if(!initTestEnv(u"opencl/math/Quotient.", FORMAT_ODS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Verify BitAnd Function
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSeriesSum()
{
    if(!initTestEnv(u"opencl/math/seriessum.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 15; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaSumIf()
{
    if(!initTestEnv(u"opencl/math/sumif.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 26; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testAddInFormulaBesseLJ()
{
    if(!initTestEnv(u"opencl/addin/besselj.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaAvedev()
{
    if(!initTestEnv(u"opencl/statistical/Avedev.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(3,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(3,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testNegSub()
{
    if(!initTestEnv(u"opencl/math/NegSub.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testMathFormulaAverageIf_Mix()
{
    if(!initTestEnv(u"opencl/math/averageif_mix.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    for (SCROW i = 0; i <= 9; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaKurt1()
{
    if(!initTestEnv(u"opencl/statistical/Kurt1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(2,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(2,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaHarMean1()
{
    if(!initTestEnv(u"opencl/statistical/HarMean1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 19; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaVarA1()
{
    if(!initTestEnv(u"opencl/statistical/VarA1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaVarPA1()
{
    if(!initTestEnv(u"opencl/statistical/VarPA1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaStDevA1()
{
    if(!initTestEnv(u"opencl/statistical/StDevA1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

void ScOpenCLTest::testStatisticalFormulaStDevPA1()
{
    if(!initTestEnv(u"opencl/statistical/StDevPA1.", FORMAT_XLS, false))
        return;
    ScDocument& rDoc = xDocSh->GetDocument();
    ScDocument& rDocRes = xDocShRes->GetDocument();
    rDoc.CalcAll();

    // Check the results of formula cells in the shared formula range.
    for (SCROW i = 1; i <= 20; ++i)
    {
        double fLibre = rDoc.GetValue(ScAddress(1,i,0));
        double fExcel = rDocRes.GetValue(ScAddress(1,i,0));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(fExcel, fLibre, fabs(0.0001*fExcel));
    }
}

ScOpenCLTest::ScOpenCLTest()
      : ScBootstrapFixture( "sc/qa/unit/data" )
{
}

void ScOpenCLTest::setUp()
{
    test::BootstrapFixture::setUp();
    // This is a bit of a fudge, we do this to ensure that ScGlobals::ensure,
    // which is a private symbol to us, gets called
    m_xCalcComponent =
        getMultiServiceFactory()->
            createInstance("com.sun.star.comp.Calc.SpreadsheetDocument");
    CPPUNIT_ASSERT_MESSAGE("no calc component!", m_xCalcComponent.is());
}

void ScOpenCLTest::tearDown()
{
    //close test env
    if(xDocSh.is())
    {
        xDocSh->DoClose();
        xDocSh.clear();
    }
    if(xDocShRes.is())
    {
        xDocShRes->DoClose();
        xDocShRes.clear();
    }

    uno::Reference< lang::XComponent >
        ( m_xCalcComponent, UNO_QUERY_THROW )->dispose();
    test::BootstrapFixture::tearDown();
}

CPPUNIT_TEST_SUITE_REGISTRATION(ScOpenCLTest);

CPPUNIT_PLUGIN_IMPLEMENT();

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
