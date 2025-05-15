/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./src/**/*.{vue,js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        primary: {
          light: '#6596ff',
          DEFAULT: '#4070f4',
          dark: '#2c4eaf'
        },
      },
      fontFamily: {
        sans: ['Noto Sans KR', 'Roboto', 'sans-serif'],
      },
    },
  },
  plugins: [],
}
