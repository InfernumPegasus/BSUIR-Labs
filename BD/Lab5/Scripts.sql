-- SELECT "ID", "FirstName", "SecondName"
-- FROM "TourAgency"."Client"
-- WHERE "ID" IN (SELECT DISTINCT "ClientID" FROM "TourAgency"."Booking");

-- SELECT b."ID", c."FirstName", c."SecondName", h."Name"
-- FROM "TourAgency"."Booking" b
-- INNER JOIN "TourAgency"."Client" c ON b."ClientID" = c."ID"
-- INNER JOIN "TourAgency"."Hotel" h ON b."HotelID" = h."ID";

-- SELECT COUNT(*) AS "TotalBookings"
-- FROM "TourAgency"."Booking";


-- SELECT "PhoneNumber", COUNT(*) AS "NumberOfClients"
-- FROM "TourAgency"."Client"
-- GROUP BY "PhoneNumber";

-- SELECT c."FirstName", c."SecondName", COUNT(b."ID") AS "TotalBookings"
-- FROM "TourAgency"."Client" c
-- LEFT JOIN "TourAgency"."Booking" b ON c."ID" = b."ClientID"
-- GROUP BY c."FirstName", c."SecondName"
-- ORDER BY "TotalBookings" DESC
-- LIMIT 3;


SELECT "Status", COUNT(*) AS "BookingCount"
FROM "TourAgency"."Booking"
GROUP BY "Status";



-- SELECT "HotelID", COUNT(*) AS "TotalRoomsBooked"
-- FROM "TourAgency"."Booking"
-- GROUP BY "HotelID";

-- SELECT "ID", "Name"
-- FROM "TourAgency"."Hotel"
-- WHERE "ID" NOT IN (SELECT DISTINCT "HotelID" FROM "TourAgency"."Booking");


-- SELECT *
-- FROM "TourAgency"."Payment"
-- WHERE "ID" NOT IN (SELECT DISTINCT "PaymentID" FROM "TourAgency"."Voucher");

-- SELECT "Date", SUM("Amount") AS "TotalAmount"
-- FROM "TourAgency"."Payment"
-- GROUP BY "Date";

-- SELECT COUNT(*) AS "DiscountedPayments"
-- FROM "TourAgency"."Payment"
-- WHERE "Discount" IS NOT NULL;


-- SELECT DISTINCT "Type"
-- FROM "TourAgency"."Tour";

-- SELECT "Type", COUNT(*) AS "TotalTours"
-- FROM "TourAgency"."Tour"
-- GROUP BY "Type";

-- SELECT "ID", "Name", "Days"
-- FROM "TourAgency"."Tour"
-- WHERE "Days" > 10;


-- SELECT "ID", "Status"
-- FROM "TourAgency"."Voucher"
-- WHERE "Status" = 'Booked'
-- AND "PaymentID" IN (SELECT "ID" FROM "TourAgency"."Payment" WHERE "Discount" IS NOT NULL);

-- SELECT COUNT(*) AS "TotalVouchers"
-- FROM "TourAgency"."Voucher";

-- SELECT "ID", "StartDate"
-- FROM "TourAgency"."Voucher"
-- WHERE "StartDate" > '2022-06-01';

