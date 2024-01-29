-- SELECT c."FirstName", c."SecondName", c."ThirdName", c."PhoneNumber", 
--        b."Date", b."Amount", b."Discount"
-- FROM "TourAgency"."Client" AS c
-- INNER JOIN "TourAgency"."Payment" AS b ON c."ID" = b."ID";

-- Booking

-- SELECT c."FirstName", c."SecondName", c."PhoneNumber",
-- 	   b."StartDate", b."EndDate", b."Status"
-- FROM "TourAgency"."Booking" AS b
-- INNER JOIN "TourAgency"."Client" AS c ON b."ClientID" = c."ID"
-- WHERE c."PhoneNumber" LIKE '+%';

-- SELECT "ID", "StartDate", "EndDate", "Status"
-- FROM "TourAgency"."Booking"
-- WHERE "Status" = 'Booked';

-- SELECT b."ID", b."StartDate", b."EndDate", b."Status", h."Name"
-- FROM "TourAgency"."Booking" AS b
-- INNER JOIN "TourAgency"."Hotel" AS h ON b."HotelID" = h."ID";

-- Client

-- SELECT "ID", "FirstName", "PhoneNumber", "PaymentID"
-- FROM "TourAgency"."Client"
-- WHERE "FirstName" LIKE 'Alexey';

-- SELECT c."ID", c."FirstName", c."PhoneNumber", b."Status"
-- FROM "TourAgency"."Client" AS c
-- INNER JOIN "TourAgency"."Booking" AS b ON c."ID" = b."ClientID";

-- SELECT "ID", "FirstName", "SecondName"
-- FROM "TourAgency"."Client"
-- WHERE "PhoneNumber" = '+7687469';

-- Hotel

-- SELECT "ID", "Name", "Address"
-- FROM "TourAgency"."Hotel"
-- WHERE "Name" LIKE 'H%';

-- SELECT "ID", "Name", "Address", "Cost"
-- FROM "TourAgency"."Hotel"
-- WHERE "Name" = 'Sheraton';

-- SELECT "ID", "Name", "Address"
-- FROM "TourAgency"."Hotel"
-- WHERE "Name" LIKE 'Hilton%';

-- Payment

-- SELECT "ID", "Date", "Amount", "Discount"
-- FROM "TourAgency"."Payment"
-- WHERE "Date" = '2022-07-21';

-- SELECT "ID", "Date", "Amount"
-- FROM "TourAgency"."Payment"
-- WHERE "Discount" < 25;

-- SELECT "ID", "Date", "Amount"
-- FROM "TourAgency"."Payment"
-- WHERE "Discount" > 50;

-- Tour

-- SELECT "ID", "Name", "Type"
-- FROM "TourAgency"."Tour"
-- WHERE "Type" LIKE '%hot';

-- SELECT "ID", "Name", "Type"
-- FROM "TourAgency"."Tour"
-- WHERE "Type" = 'Hot';

-- SELECT "ID", "Name", "Days"
-- FROM "TourAgency"."Tour"
-- WHERE "Days" > 7;

-- Voucher

-- SELECT "ID", "StartDate", "EndDate", "Status"
-- FROM "TourAgency"."Voucher"
-- WHERE "Status" = 'Booked';

-- SELECT v."ID", v."StartDate", v."EndDate", v."Status"
-- FROM "TourAgency"."Voucher" AS v
-- LEFT OUTER JOIN "TourAgency"."Booking" AS b ON v."ID" = b."VoucherID"
-- WHERE b."ID" IS NULL;

-- SELECT v."ID", v."StartDate", v."EndDate", v."Status"
-- FROM "TourAgency"."Voucher" AS v
-- INNER JOIN "TourAgency"."Payment" AS p ON v."PaymentID" = p."ID"
-- WHERE p."Discount" != 100;
