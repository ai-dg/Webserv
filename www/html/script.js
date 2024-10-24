console.log('script.js');

home = document.getElementById('home')
page1 = document.getElementById('page1')
page2 = document.getElementById('page2')

if (home)
{
    home.addEventListener('click', () => {
        document.location.href = 'index.html';
    })
}

if (page1)
{
    page1.addEventListener('click', () => {
        document.location.href = 'page1.html';
    })
}

if (page2)
{
    page2.addEventListener('click', () => {
        document.location.href = 'page2.html';
    })
}

if (page3)
    {
        page2.addEventListener('click', () => {
            document.location.href = 'page3.html';
        })
    }
