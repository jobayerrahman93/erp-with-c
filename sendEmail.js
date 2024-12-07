const nodemailer = require('nodemailer');
const process = require('process');

// Nodemailer configuration
const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
        user: 'rahmanjobayer71@gmail.com',
        pass: 'csxwkdugkftvjmzs',
    },
});

// Get arguments passed from the C program
const [email, name, month, salaryStr] = process.argv.slice(2);

// Parse the salary as a float
const totalSalary = parseFloat(salaryStr);

console.log("Arguments received:", process.argv);
console.log("Email:", email);
console.log("Name:", name);
console.log("Month:", month);
console.log("Total Salary:", totalSalary);

// Email details
const mailOptions = {
    from: 'rahmanjobayer71@gmail.com',
    to: email,
    subject: `Salary Report for ${name}`,
    text: `Hi ${name}!\n\nYour salary for the month of ${month} is: ${totalSalary}\n\nBest regards,\nNull Aerotech Limited`,
};

// Send the email
transporter.sendMail(mailOptions, (error, info) => {
    if (error) {
        console.log('Error sending email:', error.message);
    } else {
        console.log('Email sent:', info.response);
    }
});
